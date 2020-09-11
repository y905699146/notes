# TCMalloc详解

TCMalloc是谷歌开发的内存分配器，Golang的内存分配的实现采用了TCMalloc的实现。TCMalloc十分高效，极大程度的减少内存碎片

## TCMalloc的内存分配算法概览

按照分配的内存大小，TCMalloc将内存分配分为三类
- 小对象分配(0,256k]

    对于256KB以内的小对象分配，TCMalloc按大小划分了85个类别（官方介绍中说是88个左右，但我个人实际测试是85个，不包括0字节大小），称为Size Class，每个size class都对应一个大小，比如8字节，16字节，32字节。应用程序申请内存时，TCMalloc会首先将所申请的内存大小向上取整到size class的大小，比如1-8字节之间的内存申请都会分配8字节，9-16字节之间都会分配16字节，以此类推。因此这里会产生内部碎片。TCMalloc将这里的内部碎片控制在12.5%以内。


著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
- 中对象分配(256k,1MB)
- 对象分配(1MB,+)


## TCMalloc关键字
- Page : TCMalloc把整个虚拟内存划分为n个同等大小的Page,每个Page默认为8K
- Span : 连续n个Page称为一个Span
- PageHeap : 以span为单位向系统申请内存，申请到的span可能只有一个page，也可能包含n个page。可能会被划分为一系列的小对象，供小对象分配使用，也可能当做一整块当做中对象或大对象分配。

- ThreadCache : tcmalloc为每个线程都分配可一个线程本地ThreadCache，小内存从ThreadCache分配

- CentralCache : ThreadCache不够用的时候，会从CentralCache中获取空间放到ThreadCache中。
