# Go反射的用法

反射（reflect）能让我们在运行期探知对象的类型信息和内存结构，但是Go对象本身并没有类型指针，通过自身无法在运行期获取任何类型相关信息。反射操作所需要的全部信息全部来自接口变量，接口变量除了存储自身类型外，还会保存实际对象的类型数据。

```
func TypeOf(i interface{}) Type
func ValueOf(i interface{}) Value
```


除了获取对象类型外，还可以构造一些基础复合类型
```
a := reflect.ArrayOf(10, reflect.TypeOf(int32(0)))  //  [10]int32

b := reflect.MapOf(reflect.TypeOf(""),reflect.TypeOf(0)) //map[string]int

```

方法 Elem()返回指针，数组，切片，字典(值)或者通道的**基类型**
```
reflect.TypeOf(map[string]int{}).Elem()   //int
reflect.TypeOf([]int32{}).Elem()    // int32
```

通过Elem()的 NumField()方法去遍历 结构体指针的所有字段

```
type user struct {
    name string
    age int
}
 
type manager struct {
    user
    title string
}

var m manager
t := reflect.TypeOf(&m)
 if t.Kind() == reflect.Ptr {
    t = t.Elem()
}
 
for i := 0; i < t.NumField(); i++ {
    f := t.Field(i)
    fmt.Println(f.Name, f.Type, f.Offset)
    if f.Anonymous { // 输出匿名字段结构
        for x := 0; x < f.Type.NumField(); x++ {
            af := f.Type.Field(x)
            fmt.Println(" ", af.Name, af.Type)
        }
    }
}

output:
user main.user 0
  name string
  age int
title string 24
```


## Value

和 Type 获取类型信息不同，Value 专注于对象实例数据读写。


## Method
反射库提供了内置函数 make() 和 new() 的对应操作，其中最有意思的就是 MakeFunc()。可用它实现通用模板，适应不同数据类型。