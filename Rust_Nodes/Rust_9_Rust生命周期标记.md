# Rust 生命周期标记

Rust中值的生命周期和作用域相关，但是借用可以在各个函数间传递，会跨越多个词法作用域。对于函数本地声明所有权的值和借用来说，Rust编译器包含的借用检查器可以检查它们的生命周期，但是对于跨词法作用域的借用，就无法推断借用的合法性了，不合法的借用会造成悬浮指针。因此需要通过生命周期参数去显式的标记值的生命周期

```
&i32;  //引用
&'a i32;   //标注生命周期参数的引用
&'a mut i32;   //标注生命周期参数的可变引用
```

标注生命周期参数不能改变任何引用生命周期的长短，它用于编译器借用检查，例如：
```
fn the_longest<'a>(s1: &'a str,s2: &'a str) -> &'a str{
    if s1.len() > s2. len() { s1 } else { s2 }
}
fn main(){
    let s1 = String::from("Rust");
    let s1_r = &s1;
    {
        let s2 = String::from("c");
        let res = the_longest(s1_r,&s2);
        println!("{} is the longest",res);
    }
}
```
这里输入引用和输出引用都标记为'a ，表示输出引用的生命周期不长于输入引用的生命周期

多个生命周期参数标注示例：
```
fn the_longest<'a,'b:'a>(s1: &'a str,s2: &'b str) -> &'a str{
    if s1.len() > s2. len() { s1 } else { s2 }
}
```
'b : 'a 表示 b标注参数的生命周期大于'a 标注参数的生命周期。


'static 静态生命周期，'static生命周期存活于整个程序运行期间。