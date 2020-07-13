# 理解Rust中所有权和生命周期

## 所有权的概念
在常用的编程语言中，无论在堆上或者栈上分配对象都会有所有者，比如在c++中，如 果类中没有实现深拷贝，在多次拷贝下，会有多个指针指向这个内存区域，那么这块内存有多个所有者，这时候对这些指针操作都是正常的，在Golang中，通过引用传递的对象也存在这种现象，不过Golang实现了GC，如果对象引用的对象一直被使用，那么该内存不会被回收。

但是在Rust中并不允许这种行为，在Rust中一块内存区域同时只能被一个所有者拥有，并且当该所有者作用域结束的时候，所有者和它拥有的内存区域就会被销毁。

```
    let s = String::from("hello world!");
    let s1 = s;
    println!("{}",s);  //这里会编译错误，因为s的拥有对象被转移给了s1
```

如果要想继续能访问s,那么需要实现“深拷贝”的行为，在s赋值给s1时，为s1重新分配一块内存，需要手动调用clone()方法完成：
```
fn main(){
    let s = String::from("hello world!");
    let s1 = s.clone();
    println!("{} {}",s,s1);
}
```

## 移动语义
通常所有权转移发生在 函数传参，赋值语句，函数返回等：
```
fn create() -> String{ //返回值
    let s = String::from("hello world!");
    return s;
}

fn consume(s : String){   //函数传参调用
    println!("{}",s);
}

fn main(){
    let s = create();
    consume(s);  
}
```
Rust所有权转移的特点是，Rust中变量绑定的操作，默认是move语义，执行了新的变量绑定后，原来的变量就不能被使用。

## 复制语义
默认的Move语义是Rust的一个重要设计，但是有时候我们不想通过move方式，并且通过clone()的方式也比较繁琐，所以对于一些简单类型，比如int,bool赋值的时候都是默认采用复制操作。
```
fn main(){
    let v1 : isize = 0;
    let v2 = v1;
    println!("{}",v1);  //能编译通过
}
```
这里相当于 let v2 = v1.clone()
对于自定义类型，我们想要执行复制语义使用clone()时，会编译错误，自定义类型需要我们自己去实现Copy trait:
```
struct Foo{
    data : i32
}
impl Clone for Foo{
    fn clone(&self) -> Foo{
        Foo { data : self.data }
    }
}
impl Copy for Foo {}
fn main(){
    let v1 = Foo { data : 0 };
    let v2 = v1;
    println!{"{:?}",v1.data}
}
```
每个自定义类型都需要这么实现会比较麻烦，因此下面这种方式可以让编译器去帮忙实现：
```
#[derive(Copy,clone)]
struct Foo{
    data : i32
}
fn main(){
    let v1 = Foo { data:0 };
    let v2 = v1;
    println!("{:?}",v1.data);
}
```

## 什么时候使用移动语义，什么时候使用复制语义
在c++中，我们定义的对象可分为POD类型和非POD类型（Plain Old Data），意思是POD类型只包含一些基础类型，并且没有自定义的函数（比如构造，析构，拷贝，虚函数）等，这样的类型可以通过memcpy进行二进制拷贝能使其数据保持不变。在Rust中，也分为POD类型和非POD类型，Rust中的POD类型指**只包含原生类型（int,float等基础类型），并且没有自定义析构函数**，对于Box String Vec等不能按字节复制的类型，属于非POD类型，所以在Rust中，非POD使用移动语义，POD类型根据业务情况选择到底是用移动语义还是复制语义。

## 借用
Rust内存的所有权可以通过移动语义被转移，也支持借用：
借用分为只读借用和可读写借用，分别对应 & 和 &mut :
```
fn foo(v : &Vec<i32>){  
    v.push(5);   //会编译错误，push函数签名为&mut self ，
}

fn foo(v : &mut Vec<i32>){
    v.push(5);   //编译通过
}

fn main(){
    let v = vec![];
    foo(&v);
}
```

### &mut 借用规则：
1. 借用指针不能比它指向的变量存在的时间更长
2. &mut 型借用只能指向本身具有mut修饰的变量，对于**只读变量**不可以有&mut 型借用
3. &mut 型借用指针存在的时候，被借用的变量本身不可以进行操作
4. 如果只有&型借用，可以存在多个，如果有&mut型借用，只能存在一个，否则编译器会报错

## 简单的生命周期

Rust 中 Const 类型的生命周期是整个程序，它的值可以内联到代码中的任何地方

static类型生命周期也是整个程序，但是它不会内联，它存在于内存中的固定位置

大多数其他对象的生命周期只发生在块(block)的范围内，或者某个方法调用夺走了它的所有权。如果一个对象要在块中执行完后继续存在，那么需要借用(borrow)它，或者copy它，这样可以在外部作用域继续生存。
