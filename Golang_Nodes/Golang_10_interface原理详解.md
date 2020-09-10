# Golang interface实现原理

根据interface 是否包含method，底层实现上用两种struct表示：iface和eface。

eface不含method结构：
```
type eface struct {
	_type *_type
	data  unsafe.Pointer
}

type _type struct {
	size       uintptr
	ptrdata    uintptr // size of memory prefix holding all pointers
	hash       uint32
	tflag      tflag
	align      uint8
	fieldAlign uint8
	kind       uint8
	// function for comparing objects of this type
	// (ptr to object A, ptr to object B) -> ==?
	equal func(unsafe.Pointer, unsafe.Pointer) bool
	// gcdata stores the GC type data for the garbage collector.
	// If the KindGCProg bit is set in kind, gcdata is a GC program.
	// Otherwise it is a ptrmask bitmap. See mbitmap.go for details.
	gcdata    *byte
	str       nameOff
	ptrToThis typeOff
}
```

iface 表示非空interface，其中包含method
```
type iface struct {
	tab  *itab
	data unsafe.Pointer
}

type itab struct {
	inter *interfacetype
	_type *_type
	hash  uint32 // copy of _type.hash. Used for type switches.
	_     [4]byte
	fun   [1]uintptr // variable sized. fun[0]==0 means _type does not implement inter.
}
//interfacetype 包含了一些interface本身的信息，比如pkg路径和包含的方法
type interfacetype struct {
	typ     _type
	pkgpath name
	mhdr    []imethod
}

type imethod struct {
	name nameOff
	ityp typeOff
}

```

通过上面Interface的实现，可以看出interface实际是由两个成员组成，tab指向虚表，data指向实际引用的数据。虚表描绘了实际的类型信息和接口所需要的方法集。

