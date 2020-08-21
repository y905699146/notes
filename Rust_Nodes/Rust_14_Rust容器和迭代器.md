# Rust容器和迭代器

Rust标准库实现的容器有：
- Vec              可变长数组，连续存储
- VecDeque         双向队列，适用于从头部和尾部插入元素
- LinkedList       双向链表
- HashMap          哈希表
- BTreeMap         B树-map
- HashSet          
- BTreeSet
- BinaryHeap       优先队列



## 迭代器

Rust迭代器是指实现了Iterator trait的类型。
```
trait Iterator{
    type Item;
    fn next(&mut self) -> Option<Self::Item>;
}
```

比如需要给一个结构体实现一个迭代器:
```
use std::iter::Iterator;

struct Seq{
    current : i32
}

impl Seq{
    fn new() -> Self{
        Seq { current : 0 }
    }
}

impl Iterator for Seq{
    type Item = i32;

    fn next(&mut self) -> Option<i32>{
        if self.current < 100{
            self.current+=1;
            return Some(self.current);
        }else{
            return None;
        }
    }
}

fn main(){
    let mut seq = Seq::new();
    while let Some(i) = seq.next(){
        println!("{}",i);
    }
}
```
这里需要有一个变量去记录上次返回的值。

### 迭代器组合

从容器中创建迭代器有三种方法:
- iter()  创造一个Item是&T类型的迭代器
- iter_mut()  创造一个Item是&mut T类型的迭代器
- into_iter()  创造一个Item是T类型的迭代器

### 迭代器的适配器

可以通过适配器从一个迭代器中创造一个新的迭代器

```
let v = vec![1,2,3,4,5,6,7,8,9];
let mut iter = v.iter()
        .take(5)                //取前面5个元素 [1,2,3,4,5]
        .filter(|&x| x%2 == 0)  //过滤器，取x%2==0 的元素  [2,4] 
        .map(|&x| x*x)          //map定义对迭代器里面的元素处理的函数，但是惰性加载
        .enumerate()            //将迭代器里面的元素转化成元组，第一个元素是index，第二个元素是迭代器的值
```

