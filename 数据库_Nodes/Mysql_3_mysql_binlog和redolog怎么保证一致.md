# mysql binlog和redolog怎么保证一致

 ## Mysql 查询流程

 - 连接器

 判断连接是否通过，有什么权限

 - 分析器
 
 词法分析，进行语法解析，检查输入的语句是否符合语法规则

 - 优化器

 优化查询速度，比如能不能使用索引，选择使用的索引，或者联表查询的时候确定各个表的连接顺序

 - 执行器

 检查用户是否有操作指定表的权限，如果没有直接报错返回，如果有就调用存储引擎的接口，对引擎进行操作返回结果


 ## Mysql 保证事务

 Mysql 通过 WAL(write-ahead logging)技术保证事务

 在同一个事务中，每当数据库进行修改数据操作时，将修改的结果更新到内存后，会在redo log 添加一行记录记录“需要在哪个数据页上做什么修改”，并将该记录状态置位prepare，等到commit提交事务后，会将此次事务在redo log添加的记录状态都置为commit状态，最后落盘时，会将redo log中状态为commit的记录修改都写入磁盘


## redo log 记录方式

redo log 大小是固定的，是一个环状结构，有两个指针，一个是write pos表示当前记录的位置，另一个是check point 表示将日志记录写磁盘的位置， write pos和 check point之间是空着的部分。当 write pos 追上 check point时。就先停下记录，先推动check point移动。空出位置在记录新的日志。

redo log的缺点是日志记录落盘后，日志就会被覆盖掉，无法用于数据回滚/数据恢复


## binlog 和 redo log怎么保持一致

### binlog 优势
- binlog是server层实现的，意味着所有引擎都可以使用binlog日志

- binlog通过追加的方式写入的，可通过配置参数max_binlog_size设置每个binlog文件的大小，当文件大小大于给定值后，日志会发生滚动，之后的日志记录到新的文件上。

- binlog有两种记录模式，statement格式的话是记sql语句， row格式会记录行的内容，记两条，更新前和更新后都有。

### 如何保证一致

binlog和redo log必须保持一致，不允许出现binlog有记录但redolog没有的情况，在同一个事务中，redolog有prepare和commit两种状态，所以，在redolog状态为prepare时记录binlog可保证两日志的记录一致

流程：
 
 1. mysql 提交一个 update 语句
 2. server层进行分析后，调用存储层的接口进行修改
 3. innodb将修改结构更新到内存中
 4. 修改好了，准备提交事务，在redo log写入状态为prepare
 5. 写入 binlog 
 7. 开始commit
 8. 将redo log中的prepare状态变成 commit