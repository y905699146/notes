# Rust类型转换

Rust提供两种方式的类型转换：
- as ,用于安全的转换
- transmute , 用于任意的转换，是Rust中最危险的功能


转换可以用来下面的例子：

- 传递性： 当 T_1 可以强制转换为 T_2 且 T_2 可以强制转换为 T_3 时，T_1 就可以强制转换为 T_3

- 指针弱化


    去除引用可变性
    - &mut T 到 &T

    去除裸指针的可变性
    - *mut T 到 *const T

    引用也能被强转为裸指针：
    - &T 到 *const T
    - &mut T 到*mut T

- Unsize: 
    如果 T 实现了 CoerceUnsized<U>，那么 T 可以强制转换为 U

- 强制解引用：

    如果 T 可以解引用为 U（比如 T: Deref<Target=U>），那么 &T 类型的表达式 &x 可以强制转换为 &U 类型的 &*x

## 数值转换
```
```

## 指针转换