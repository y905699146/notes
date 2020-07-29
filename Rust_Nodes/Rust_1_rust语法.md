# rust 语法

## Vectors

Vector是一个动态的数组。数据存储在堆上，标准库实现为Vec<T>。

创建方式：
```
let v = vec![1,2,3,4,5];
let v = vec![0; 10];  // 10个0
```
访问Vec的元素可以通过下标访问，但是下标的类型必须是 usize 。

访问越界的元素会直接panic,如果想处理越界错误而不是直接panic可以这样实现:
```
let v = vec![1, 2, 3];
match v.get(7) {
    Some(x) => println!("Item 7 is {}", x),
    None => println!("Sorry, this vector is too short.")
}
```

遍历Vec的方式：
```
let mut v = vec![1,2,3,4,5];
for i in &v {
    println!("A reference to {}", i);
}

for i in &mut v {
    println!("A mutable reference to {}", i);
}

for i in v {
    println!("Take ownership of the vector and its element {}", i);
}
///////////////////////////////////////////////////////////
let v = vec![1, 2, 3, 4, 5];

for i in &v {
    println!("This is a reference to {}", i);
}

```


## 循环操作
Rust 目前提供 3 种方法来进行一些迭代操作。他们是loop，while和for。每种方法都有自己的用途。
- loop 用法：
```
loop{
   // do something;
}
```

- while 用法：
```
where true{

}
```

- for 用法：
```
for i in 0..10{

}

for var in expression{

}
//expression 为迭代器
```

- 遍历时想要获取当前元素的下标可以通过 .enumerate()函数
```
for (index, value) in (5..10).enumerate() {
    println!("index = {} and value = {}", index, value);
}

output:
index = 0 and value = 5
index = 1 and value = 6
index = 2 and value = 7
index = 3 and value = 8
index = 4 and value = 9
```

```
//对于迭代器对象：
let lines = "hello\nworld".lines();

for (linenumber, line) in lines.enumerate() {
    println!("{}: {}", linenumber, line);
}
output:
0: hello
1: world
```

- break 和 continue 和 c语言的用法类似，用来退出循环

- 循环标签，默认break 和 continue 作用于当前循环，如果在多层循环中，需要跳到外层循环，可以这么实现：
```
'outer: for x in 0..4 {
        'inner: for y in 0..4 {
            if x % 2 == 0 { continue 'outer; } 
            if y % 2 == 0 { continue 'inner; } 
            println!("x: {}, y: {}", x, y);
        }
    }

output:
    x: 1, y: 1
    x: 1, y: 3
    x: 3, y: 1
    x: 3, y: 3
```

## DST 和 胖指针
DST： “动态大小类型”，指在编译期无法确定占用空间大小的类型，由于Rust编译器对类型有严格要求，需要在编译时已知使用内存的大小，因此DST类型编译器是不接受的，为了让编译器接受DST类型，需要通过 “ 胖 指针 ”

胖指针是一种特殊的指针：
- 指向静态sized类型值的常规指针；对于数组[i32; 3]而言，& [i32; 3]就是常规指针；
- 指向动态sized类型值的胖指针：使用的内存空间是常规指针所使用的内存空间的两倍，切片(& [i32])这种指针是胖指针(fat pointer)；trait对象和切片变量的类型其实是胖指针类型！



## 全局变量

Rust中可以通过 static关键字声明全局变量,static 声明的变量的生命周期是整个程序
```
static GLOBAL: i32 = 0;
```
static 声明语句，必须显式标明类型，不允许类型自动推导。

Rust 中还可以用 const 关键字做声明。
```
const GLOBAL: i32 = 0;
```
使用 const 声明的是常量，而不是变量。它与 static 变量的最大区别在于，编译器并不一定会给 const 常量分配内存空间，在编译过程中，它很可能会被内联优化。以 const 声明一个常量，也不具备类似 let 语句的模式匹配功能。

Rust 对全局变量的使用有许多限制。包括：
- 全局变量必须在声明的时候马上初始化
- 全局变量的初始化必须是简单赋值，不能包括复杂的表达式、语句和函数调用
- 带有 mut 修饰的全局变量，在使用的时候必须使用 unsafe

## 数组类型

特点：
- 数组大小固定
- 元素均为同类型
- 默认不可变

数组类型签名为 ： [T;N]  T是一个泛型标记，表示数据类型，N是长度

## 范围类型

Rust内置了范围类型，包括左闭右开和全闭两种区间：
```
for i in (1..5)    //1,2,3,4
for i in (1..=5)   //1,2,3,4,5
```

## 切片类型
切片(Slice)是对一个数组(固定大小数组和动态数组)的引用片段，有利于安全有效的访问数组的一部分，而不需要拷贝。
```
let arr : [i32; 5]=[1,2,3,4,5];
let t = &arr[1..2];  //t是一个切片
```

## 字符串类型
String是一个可变的、堆上分配的UTF-8的字节缓冲区。

str是一个不可变的固定长度的字符串，如果是从String解引用而来的，则指向堆上，如果是字面值，则指向静态内存。

&String 是String的borrowed类型，这只不过是一个指针类型，可以传递而不放弃ownership。事实上，一个&String可以当做是&str。

## 原生指针类型

Rust提供多种类型的指针，包括引用，原生指针，函数指针，和智能指针。

引用本质是非空指针，引用主要用于safe Rust中;

原生指针可以操控地址，主要用于unsafe Rust中；



## never类型

never类型，即 ! 该类型用于表示永远不可能有返回值的计算类型，比如现场退出时候不可能有返回值。

never类型属于实验特性，使用需要使用 #![feature(never_type)]，所以需要在Nightly版本下使用该特性。

## struct impl trait的作用

struct定义结构体
```
struct People{
    name : String,
    weight : i32
}
```
impl 关键字定义结构体成员方法
```
impl People{
    fn new(n :String,w : i32) -> People{
        People{
            name : n,
            weight : w
        }
    }
    fn printInfo(&self){
        println!{"{},{}",self.name,self.weight};
    }
}
```
成员函数操作成员变量需要使用self参数，有这样几种类型：
- self，允许实现者移动和修改对象，对应的闭包特性为FnOnce
- &self，既不允许实现者移动对象也不允许修改，对应的闭包特性为Fn
- &mut self，允许实现者修改对象但不允许移动，对应的闭包特性为Fn Mut
- 不含self参数的关联函数称为静态方法 (static method)

trait用来定义接口：
```
trait Arae{
    fn area(&self) -> f64;
}
//struct定义一个类
struct Circle{
    r : f64
}
//让具体类实现接口
impl Area for Circle{
    fn area(&self) -> f64{
        return (3.14 * self.r)
    }
}
```



## 复合数据类型

### 元组（tuple）
和python类型 ，类似 (A,B,C,D)这种结构，固定长度，可以根据索引来获取元组内部的值：
```
let tuple : (&'static str, i32 , char) = ("hello",5,'c');
print!{"{:?},{:?}",tuple.0,tuple.1} 
```
元组支持模式解构。

当元组只有一个值的时候，定义为 (0,)，加逗号是为了和其他值区分

### 结构体
Rust 分为三种结构体

- 具名结构体


- 元组结构体


- 单元结构体

### 枚举体
```
//无参数枚举体
enum Number{
    Zero,
    One,
    Two,
}
//类C枚举体
enum Color{
    Red = 0xff0000,
    Green = 0x00ff00,
    Blue = 0x0000ff,
}
//带参数枚举体
enum Ipaddr{
    V4(u8,u8,u8,u8),
    V6(String),
}
```

## 常用集合类型
- 线性序列 ： 向量(Vec)，双端队列(VecDeque)，链表(LinkedList)
- Key-Value映射表: 无序哈希表(HashMap),有序哈希表(BTreeMap)
- 集合类型: 无序集合(HashSet)，有序集合(BTreeSet)
- 优先队列: 二叉堆(BinaryHeap)

## 智能指针
Rust 中的值默认被分配到栈内存。可以通过Box<T>将值放到堆中，Box<T>指向类型为T的堆内存分配值的智能指针。当Box<T>超出作用域时，将调用析构函数，释放内存，可以通过解引用操作来获取Box<T>中的T。

## 泛型
