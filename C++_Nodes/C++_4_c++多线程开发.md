# C++多线程开发

## 线程创建
```
void func() {}

std::thread t(func);
t.join()  //阻塞
// t.detach()  //不阻塞，但是要注意thread离开作用域会析构，如果此时线程没有执行完会发生错误



```