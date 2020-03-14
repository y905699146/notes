# plan9 汇编总结

Golang可以使用下面命令将Go代码生成汇编代码：
```
go tool compile -N -L -S xxx.go
-N  禁止编译优化
-L  禁止内联
-S  生成汇编代码

```
