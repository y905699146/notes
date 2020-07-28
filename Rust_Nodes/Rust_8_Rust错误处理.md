# Rust的错误处理机制

- Option< T >
    
    Option类型定义在标准库中：
    ```
    enum Option< T >{
        None,
        Some(T),
    }
    ``` 
    Option类型是一个 Rust 类型系统用于表达不存在的可能性（possibility of absence）的方式。将不存在的可能性编码进类型系统是一个重要概念，因为它会强迫编译器处理不存在的情况。

- Result< T >

    ```
    #[must_use]   //这个属性表示如果对程序中的Result<T,E>没有处理，则发出警告来提示开发者必须处理相应的错误
    enum Result<T,E>{
        Ok(T),
        Err(E),
    }
    ```
    Result是Option的高级版本，相比于像Option那样表示不存在的可能性，Result表示错误的可能性。通常，错误用来解释为什么一些计算会失败。严格的说这是一个更通用的Option。


- Panic

- abort