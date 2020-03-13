# GC unsafe pointer理解

Golang 标准库提供了 unsafe这个包，可以用来提供不同指针类型的转换操作，还可以操作指针的偏移来获取数据

unsafe主要内容如下：
```
package unsafe 

type ArbitraryType int

type Pointer *ArbitraryType
//返回变量在内存中占用的字节数，如果是slice,
func Sizeof(x ArbitraryType) uintptr

func Offsetof(x ArbitraryType) uintptr

func Alignof(x ArbitraryType) uintptr
```