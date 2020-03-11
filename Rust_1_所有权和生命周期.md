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