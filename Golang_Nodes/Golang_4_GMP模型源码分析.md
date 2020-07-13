# GMP 模型源码分析
 
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

    sudogcache []*sudog
	sudogbuf   [128]*sudog
}
```

P的状态
```
const (
    _Pidle    = iota //表示P当前没有和任何M相关联
	_Prunning        //表示当前P正在和某个M相关联
	_Psyscall        //表示当前P运行的那个G在进行系统调用
	_Pgcstop         //表示当前P需要停止调度，在进行GC扫描前会将全局P列表置于此状态
	_Pdead           //表示当前P不会在使用，在调用runtime.GOMAXPROCS函 
                     //数减少P数量时，会把多余的P置于此状态
)
```


G的部分结构（位于src/runtime/runtime2.go）:
```
type g struct{
    stack           stack
    stackguard0     uintptr // offset known to liblink
	stackguard1     uintptr // offset known to liblink
    m               *m      //当前M
}
```

G的状态
```
const (
    _Gidle = iota                //表示当前G刚被初始化，但是还未运行
    _Grunnable                   //表示当前G在可运行队列中等待运行
    _Grunning                    //表示当前G正在运行
    _Gsyscall                    //表示当前G正在进行某个系统调用
    _Gwaiting                    //表示当前G正在阻塞
    _Gmoribund_unused            // 未使用
    _Gdead                       //表示当前G正在闲置
    _Genqueue_unused             // 未使用
    _Gcopystack                  //表示当前G正在进行栈伸缩
    /* 这些状态在 GC 扫描时发生 */
    _Gscan         = 0x1000
	_Gscanrunnable = _Gscan + _Grunnable // 0x1001
	_Gscanrunning  = _Gscan + _Grunning  // 0x1002
	_Gscansyscall  = _Gscan + _Gsyscall  // 0x1003
	_Gscanwaiting  = _Gscan + _Gwaiting  // 0x1004
)
```


## 上下文切换

由于中断处理，多任务处理，用户态切换等原因会导致 CPU 从一个线程切换到另一个线程，切换过程需要保存当前进程的状态并恢复另一个进程的状态。

上下文切换的代价是高昂的，因为在核心上交换线程会花费很多时间。上下文切换的延迟取决于不同的因素，大概在在 50 到 100 纳秒之间。考虑到硬件平均在每个核心上每纳秒执行 12 条指令，那么一次上下文切换可能会花费 600 到 1200 条指令的延迟时间。实际上，上下文切换占用了大量程序执行指令的时间。

如果存在跨核上下文切换（Cross-Core Context Switch），可能会导致 CPU 缓存失效（CPU 从缓存访问数据的成本大约 3 到 40 个时钟周期，从主存访问数据的成本大约 100 到 300 个时钟周期），这种场景的切换成本会更加昂贵。

- 上下文切换代价小： Goroutine 上下文切换只涉及到三个寄存器（PC / SP / DX）的值修改；而对比线程的上下文切换则需要涉及模式切换（从用户态切换到内核态）、以及 16 个寄存器、PC、SP…等寄存器的刷新；
- 内存占用少：线程栈空间通常是 2M，Goroutine 栈空间最小 2K；



Golang 程序中可以轻松支持10w 级别的 Goroutine 运行，而线程数量达到 1k 时，内存占用就已经达到 2G。