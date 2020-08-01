# Rust同步和并发原理

## 线程管理：
    std::thread 模块定义了管理线程的各种函数和一些底层同步原语
    std::sync 模块定义了锁，Channel,条件变量和屏障

## 创建线程(和c++类似)
```
let th = thread::spawn(move || { xxxxx;  });
th.join()
```
Rust中主线程和子线程是乱序执行的。直接用thread::spawn生成的线程，默认没有名称，并且栈大小为2MB。如果需要修改大小，需要使用thread::Builder结构体来创建可配置的线程。

```
use std::thread::{Builder,current};

let threadName = format!("child-{}",id);
let size: usize = 3 * 1024;
let builder = Builder::new().name(threadName).stack_size(size);
let child = builder.spawn(move ||{ xxxxx; });

```

## 线程本地存储 （thread_local）
每个线程都有独立的存储空间，可以存放其他线程无法访问的数据。
```

```