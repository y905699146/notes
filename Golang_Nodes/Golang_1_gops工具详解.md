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