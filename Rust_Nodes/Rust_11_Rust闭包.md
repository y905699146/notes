# 闭包

闭包 通常是一个持有外部环境变量的函数。
Rust 闭包定义(和C++中的类似)：
```
let a = |x : i32 | -> i32{ x+1 }; 

```

闭包可以引用外部环境变量：
```
fn main() {
    let num = 2;
    let plus_one = |x: i32| -> i32 { x + num };//在作用域引用外部的num
    let a = plus_one(1);
    println!("{}",a);
}
```
但是闭包在借用外部环境变量时，外部不能借用，除非闭包脱离当前作用域
```
et mut num = 5;
let plus_num = |x: i32| x + num;

let y = &mut num;  //报错
```

正确做法
```
et mut num = 5;
{
    let plus_num = |x: i32| x + num;      //脱离当前作用域之后可以使用
}
let y = &mut num;
```