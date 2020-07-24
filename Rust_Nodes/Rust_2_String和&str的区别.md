# String,&str区别

String是一个可变的、堆上分配的UTF-8的字节缓冲区。

str是一个不可变的固定长度的字符串，如果是从String解引用而来的，则指向堆上，如果是字面值，则指向静态内存。

&String 是String的borrowed类型，这只不过是一个指针类型，可以传递而不放弃ownership。事实上，一个&String可以当做是&str。