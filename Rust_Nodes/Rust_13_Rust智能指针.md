# Rust智能指针

## Box< T >指针
Box< T >将数据放堆上而不是栈上，栈上保留指向堆数据的指针，一般有三种用法：
- 比如定义一个链表节点：

    ```
    struct Node{
        data : i32,
        next : Node,
    }
    ```

    这样会编译失败，因为编译时不知道大小

    可以换成这样的写法：

    ```
    struct Node{
        data : i32,
        next : Box<Node>,
    }
    ```
    这样可以编译通过，但是无法使用，因为Rust的变量需要初始化，所以需要定义一个新的Node给next,但是新定义的Node也需要next，所以需要用Option解决：

    ```
    #[derive(Debug)]
    struct Node{
        data : i32,
        next : Option<Box<Node>>,
    }

    let node = Node{
        data : 1,
        next : None,
    };
    ```
- 当有大量数据并希望在确保数据不被拷贝的情况下转移所有权的，保存在堆上的时候，转移所有权只需要拷贝栈上指针即可

- 当希望拥有一个值并只关心它的类型是否实现了特定的trait而不是具体类型的时候

## Rc < T > 类似C++ shared_ptr
Rust提供Rc<T>类型满足多所有权的需要，通过引用计数实现，Rc<T>单线程引用计数指针，不是线程安全的类型

## Weak < T > 
类似C++ weak_ptr

## Arc< T >  原子引用计数，线程安全
跟Rc< T >一样，不过可以在线程间安全的共享

## Cell< T >

## RefCell< T >
只读引用不可以修改值，但是有时候需要修改引用对象的值，需要用RefCell< T >
```
let x = RefCell::new(4i32);
    {
        let mut y =x.borrow_mut();
        *y = 10;
        println!("{}",y);   //10
    }
    println!("{:?}",x);   //10
```


## UnsafeCell< T >

