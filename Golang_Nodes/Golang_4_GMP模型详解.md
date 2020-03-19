# GMP 模型详解
 
 - M : machine的缩写。一个M代表一个内核线程，或称为“工作线程”。
 - P : processor的缩写。一个P代表一个Go代码片段所需要的资源（或称为“上下文环境”）
 - G : goroutine的缩写。一个G代表一个Go代码片段，前者是对后者的一种封装。

简单来说：
    一个**G**的执行环境需要**P**和**M**的支持。一个M和P关联后，就形成了一个有效的G运行环境（内核线程+上下文环境）。每个P都会包含一个可运行的G的队列。该队列中的G会被依次传递给与本地P关联的M，并获得运行时机。

M的部分结构(位于src/runtime/runtime2.go)：
```
type m struct{
    g0        *g       // 系统启动时创建，用来执行一些运行时任务
    mstartfn  func()   // 表示系统监控，GC辅助，M自旋的函数
    curg      *g       // 正运行在M上的G指针
    p         puintptr // 指向当前的P
    nextp     puintptr // 暂存与M有潜在关联的P，用来做”预联“
    spinning  bool     // 用来判断释放在寻找可运行的G，在寻找过程中M会处于自旋状态
    lockedg   *g  // 用来将某个G与M绑定，常用在CGO中
}
``` 
M存在一个全局的M列表（runtime.allm）,所有创建好的M都会放到这个列表中。这个列表的目的是为了系统获取所有M的信息，同时为了防止M被当作垃圾回收掉。运行的M有可能会被停止，比如触发GC的时候，会被放到调度器的空闲M列表（runtime.sched.midle）。当运行一个未被使用的M时，运行系统会尝试从该列表获取。M是否空闲，看它是否在这个空闲列表里面。M的最大数量可以限制，通过runtime/debug里面的SetMaxThreads函数。

P的部分结构（位于src/runtime/runtime2.go）:
```
type p struct{
    id         int32
    status     uint32    //p 的状态
    runqhead   uint32
    runqtail   uint32
    runq       [256]guintptr

    runnext    guintptr

    gFree      struct{   //闲置g的队列
        gList
        n int32
    }
}
```


G的部分结构（位于src/runtime/runtime2.go）:
```
type g struct{
    
}
```