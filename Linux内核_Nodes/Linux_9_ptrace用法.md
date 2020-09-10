# ptrace用法

首先看一下ptrance函数
```
long ptrace(enum __ptrace_request request,
            pid_t pid,
            void *addr,
            void *data);

```