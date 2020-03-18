# plan9 汇编总结

Golang可以使用下面命令将Go代码生成汇编代码：
```
go tool compile -N -L -S xxx.go
-N  禁止编译优化
-L  禁止内联
-S  生成汇编代码
```

## plan9汇编中的寄存器
1. FP伪寄存器



2. SP寄存器
3. SB寄存器
4. PC寄存器
5. BP寄存器
6. 通用寄存器


## plan9 常见指令的分析



## 