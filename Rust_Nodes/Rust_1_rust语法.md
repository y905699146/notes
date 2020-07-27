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