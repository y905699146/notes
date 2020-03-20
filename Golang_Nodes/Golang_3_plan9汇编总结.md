# plan9 汇编总结

Golang可以使用下面命令将Go代码生成汇编代码：
```
go tool compile -N -L -S xxx.go
-N  禁止编译优化
-L  禁止内联
-S  生成汇编代码
```

## plan9汇编中的寄存器
- FP寄存器

    使用形如 symbol+offset(FP) 的方式，引用函数的输入参数。例如 arg0+0(FP)，arg1+8(FP)，使用 FP 不加 symbol 时，无法通过编译，在汇编层面来讲，symbol 并没有什么用，加 symbol 主要是为了提升代码可读性。另外，官方文档虽然将伪寄存器 FP 称之为 frame pointer，实际上它根本不是 frame pointer，按照传统的 x86 的习惯来讲，frame pointer 是指向整个 stack frame 底部的 BP 寄存器。假如当前的 callee 函数是 add，在 add 的代码中引用 FP，该 FP 指向的位置不在 callee 的 stack frame 之内，而是在 caller 的 stack frame 上


- SP寄存器
    
    SP寄存器指向当前栈帧



- SB寄存器


- PC寄存器
   
   类似×86平台下得ip寄存器，amd64平台下得rip寄存器，指向正在执行的指令。

### ×86寄存器和plan9寄存器的对应关系
| ×86  |  plan9 |  用途 |
| ---- |  ----  |  ---  |
| rax  |   AX   |       | 
| rbx  |   BX   |       | 
| rcx  |   CX   |       | 
| rdx  |   DX   |       | 
| rdi  |   DI   |       | 
| rsi  |   SI   |       | 
| rbp  |   BP   |       | 
| rsp  |   SP   |       | 
| r8   |   R8   |       | 
| r9   |   R9   |       | 
| r10  |   R10  |       | 
| r11  |   R11  |       | 
| r12  |   R12  |       | 
| r13  |   R13  |       | 
| r14  |   R14  |       | 
| rip  |   PC   |       | 


## plan9 基本指令的分析
go指令大全: [plan9指令](https://golang.org/src/cmd/internal/obj/x86/anames.go)

1. 栈操作

    plan9中栈操作没有使用push,pop,而是采用sub跟add SP
    ```
    SUBQ	$248, SP  //由于栈是高地址到地址，扩栈是对SP做减法
    ADDQ	$248, SP  //清除函数栈帧
    ```

2. 数据转移操作

    ```
    MOVB    //转移一字节数据
    MOVW    //转移二字节数据
    MOVD    //转移四字节数据
    MOVQ    //转移八字节数据
    ```
3. 计算指令

    ADDQ AX,BX     //加法指令  => BX += AX
    SUBQ AX,BX     //减法指令  => BX -= AX
    IMULQ AX,BX    //乘法指令  => BX *= AX
    IDIVQ CX       //除法指令


4. 跳转指令
   
   无条件跳转：

   JMP addr    //跳到某个地址
   JMP label   //跳转到某个标签
   JMP 2(PC)   //以当前指令为基础，向前跳转2行
   JMP -2(PC)  //以当前指令为基础，向后跳转2行

   有条件跳转

   JL           //小于
   JLZ          //小于等于
   JEQ           //等于
   JNE          //不等于
   JG           //大于
   JGE          //大于等于

5. 地址操作



    地址运算指令是lea指令，golang里面对应指令是LEAQ：
```
LEAQ (BX)(AX*8),CX    => CX = BX + (AX * 8)
```


## go 函数调用
- 函数声明：
 
  ```
    TEXT symbol(SB), [flags,] $framesize[-argsize]
  ```
  函数的定义部分由5个部分组成：TEXT指令、函数名、可选的flags标志、函数帧大小和可选的函数参数大小。