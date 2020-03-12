# gops原理和使用介绍

gops是google开源的一个go程序调试工具，可以在运行中获取go程序的各种状态~ 比起go标准库自带的pprof更加方便，快捷。

获取方式：
```
go get -u github.com/google/gops
```

可以通过gops help查看帮助文档：
```
gops is a tool to list and diagnose Go processes.

Usage:
  gops <cmd> <pid|addr> ...
  gops <pid> # displays process info
  gops help  # displays this help message

Commands:
  stack      Prints the stack trace.
  gc         Runs the garbage collector and blocks until successful.
  setgc      Sets the garbage collection target percentage.
  memstats   Prints the allocation and garbage collection stats.
  version    Prints the Go version used to build the program.
  stats      Prints runtime stats.
  trace      Runs the runtime tracer for 5 secs and launches "go tool trace".
  pprof-heap Reads the heap profile and launches "go tool pprof".
  pprof-cpu  Reads the CPU profile and launches "go tool pprof".

All commands require the agent running on the Go process.
"*" indicates the process is running the agent.
```

一般来说如果只是简单的分析程序系统性能情况，gops可以做到非侵入式，类似Linux下的ps指令，但是如果需要细致的对程序性能进行分析，需要在程序在引入agent包:
`"github.com/google/gops/agent"`  
agent中含有三个参数：
```
type Options struct{
  Addr string      //可选，设置gops的监听地址，如":9090".就可以在
                   //该端口上分析go程序
  ConfigDir string   //可选，用来存放统计数据的目录
  ShutDownCleanup  bool   //可选，设置为true时程序关闭时会清空数据
}
```
在主函数中引入：
```
if err := agent.Listen(agent.Options{}); err != nil { //Options设置可选参数
		log.Fatal(err)
	}
	time.Sleep(time.Hour)
```
就可以使用更强大的调试功能

## gops 各个参数的用法
1. gops :
  列出本机所有的go程序 ，带 * 号表示引入 agent的程序
2. gops < pid >
  查看 pid 对应go程序的基本信息
3. gops < tree >
  以目录树的形式展示所有go程序
4. gops < stack > < pid >
  显示 pid 对应go程序的堆栈信息
5. gops < memstats > < pid >
  显示 pid 对应go程序的内存统计情况
6. gops < gc > < pid >
  显示 pid 对应go程序的垃圾回收信息
7. gops setgc                    
  设置 pid 对应go程序的GC目标百分比
8. gops < version > < pid > 
  查看指定程序的Go版本号
9. gops stats < pid >
  查看 pid 对应程序的goroutine数量，GOMAXPROCS信息
10. gops pprof-cpu < pid > 
  显示CPU的性能分析数据
11. gops pprof-heap < pid >
  显示heap的性能分析数据
12. gops trace < pid > 
  追踪程序运行5s，生成可视化报告，并在浏览器中查看


# gops 实现原理
 gops 的工作原理是通过扫描系统当前所有的进程的可执行文件，确认是否为 golang 程序并获取编译器版本，对于 golang 程序进程则进一步检查 {$GOPS_CONFIG_DIR}/{$PID} 或 {$HOME}/.config/gops/{$PID} 下是否有对应文件，如果有则说明该进程引入了 agent，则在显示该进程时添加额外标记。gops 从操作系统接口获取进程的运行信息，从 agent 输出的文件中读取 golang runtime 信息。