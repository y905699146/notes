# GC unsafe pointer理解

Golang 标准库提供了 unsafe这个包，官方定义如下：
```
	- A pointer value of any type can be converted to a Pointer.
	- A Pointer can be converted to a pointer value of any type.
	- A uintptr can be converted to a Pointer.
	- A Pointer can be converted to a uintptr.
```
意思是可以用来提供不同指针类型的转换操作操作，还可以操作指针的偏移来获取数据

unsafe主要内容如下：
```
package unsafe 

type ArbitraryType int

type Pointer *ArbitraryType

//返回变量在内存中占用的字节数，如果是slice,以slice内存分配为准
func Sizeof(x ArbitraryType) uintptr

//返回结构体中指定属性的偏移量，参数必须为struct类型
func Offsetof(x ArbitraryType) uintptr
 
//返回变量对齐的字节数
func Alignof(x ArbitraryType) uintptr
```

unsafe 中的Sizeof类似c语言中的sizeof，只不过c语言中的sizeof是关键字，unsafe中的Sizeof是一个函数接口，Sizeof函数计算的是对象占用内存的字节数，对应**指针类型**在64位下是8个字节，对于Slice是实际分配的内存数，比如：
` s := make([]int,0,10) `
实际占用12个字节，和Slice的内存分配有关。
对于**字符类型**，比如：
` unsafe.Sizeof("hello,world")` 无论多长的字符串，都是16个字节，8个字节存储的是字符串的长度，后8个字节存储的是字符串的地址，通常指向 .rodata 段的内存区域。对于 **map，channel** 对象，unsafe.Sizeof求得长度都是8。


Offsetof参数主要是用来获取结构体指定属性的偏移量，参数必须为结构体中的属性：
```
s := struct{
    a byte
    b int32
    c float64
    d int64
}{0,0,1.1,0}
fmt.Println(unsafe.Offsetof(s.a))   //0
fmt.Println(unsafe.Offsetof(s.b))   //4
fmt.Println(unsafe.Offsetof(s.c))   //8
fmt.Println(unsafe.Offsetof(s.d))   //16
```
可以通过偏移量转化对应的指针去修改数据。

Alignof函数主要用来计算变量对齐的字节数量，比如:
```
byte     1
bool     1
int8     1
int32    4
int64    8
string   16
float64  8
struct   8
map      8
Slice    8
chan     8
```