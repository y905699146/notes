# trait功能详解

rust 中 Trait类似其他语言里面的抽象接口，接口可以被继承，Trait里面的抽象方法可以被重写。Trait包含下面三部分：

- functions (方法)
- types (类型)
- constants(常量)

所有trait都定义了一个隐含的Self,指向实现该Trait的类型