# 逆向分析-PE文件格式

## PE 文件种类
- 可执行系列 : EXE , SCR
- 库系列    : DLL,OCX,CPL,DRV 
- 驱动文件系列 : SYS,VXD
- 对象文件系列 : OBJ

## PE 头

- 虚拟内存地址(VA):PE文件中的指令被装入内存后的地址
- 相对虚拟内存地址(RVA):相对虚拟地址是内存地址相对于映射基址的偏移量
- 文件偏移地址(FOA):数据在PE文件中的地址叫文件偏移地址，这是文件在磁盘上存放相对于文件开头的偏移
  
PE文件需要做磁盘到内存的映射，当PE文件加载到进程虚拟内存的特定位置时，该位置可能已经加载了其他PE文件。所以PE头的的内部信息大多以RVA的形式存在。VA和RVA有如下对应关系：
    RVA + ImageBase = VA  (ImageBase为基准位置)

PE文件加载到内存中时，也需要完成RVA到RAW的映射：
    RAW = RVA　－　VA　＋　PointerToRawData
    PointerToRawData是PE文件中的节区起始地址。
    VA是RAW中对于节区的起始地址


## IAT （导入地址表）


## EAT　（导出地址表）