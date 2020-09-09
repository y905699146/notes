# kafka 原理解析

- Topic : 类似关系数据库的表，逻辑上的概念
- Partiton :  跟数据库的分区差不多，为了提高性能
- Producer : 生产者，发送数据
- Consumer : 消费者，从Kafka里面读取数据
- Message  : 消息
- Replica  ：副本，每个Partition可以设置多个副本，可以选取一个副本作为Leader，其余的作为Follower,生产者在发送数据的时候，是直接发送到leader partition里面 ，然后follower partition会去leader那里自行同步数据，消费者消费数据的时候，也是从leader那去消费数据的 。

- Consumer Group 消费者组

- controller kafka是主从架构，主节点就叫controller,其余为从节点，controller是需要和zookeeper进行配合管理整个Kafka集群




## kafka怎么和zookeeper配合工作


kafka严重依赖于zookeeper集群（所以之前的zookeeper文章还是有点用的）。所有的broker在启动的时候都会往zookeeper进行注册，目的就是选举出一个controller，这个选举过程非常简单粗暴，就是一个谁先谁当的过程，不涉及什么算法问题。

那成为controller之后要做啥呢，它会监听zookeeper里面的多个目录，例如有一个目录/brokers/，其他从节点往这个目录上注册（就是往这个目录上创建属于自己的子目录而已） 自己，这时命名规则一般是它们的id编号，比如/brokers/0,1,2

注册时各个节点必定会暴露自己的主机名，端口号等等的信息，此时controller就要去读取注册上来的从节点的数据（通过监听机制），生成集群的元数据信息，之后把这些信息都分发给其他的服务器，让其他服务器能感知到集群中其它成员的存在 。

此时模拟一个场景，我们创建一个主题（其实就是在zookeeper上/topics/topicA这样创建一个目录而已），kafka会把分区方案生成在这个目录中，此时controller就监听到了这一改变，它会去同步这个目录的元信息，然后同样下放给它的从节点，通过这个方法让整个集群都得知这个分区方案，此时从节点就各自创建好目录等待创建分区副本即可。这也是整个集群的管理机制。