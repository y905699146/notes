​*本文作者-麦客存储 原创作品 转载需征得作者同意*

上文 [Lotus-Storage-Miner架构解析](https://juejin.im/post/6864419056360488973)介绍了挖矿软件的主要架构，这篇文章将从源码角度分析Filecoin的共识和出块的流程。


## 共识机制

对于区块链来说，一个很重要的点是它的共识机制，共识机制对于分布式系统来说是必不可少的，因为共识机制决定谁拥有出块权,它能够保证分布式网络的参与者都以唯一的目标进行协作，协调多方达到同样的结果，保证整个网络能够持续稳定的运行。目前在区块链公链中主要的共识机制有:

- PoW     工作量证明，干的越多，收的越多
- PoS	  权益证明，持有越多，获得越多
- DPoS    由代理人进行投票

## Filecoin的共识机制
对于Filecoin网络来说，并不是采取上述的共识机制，而是采取的一种非交互式的方式去实现的，被称为预期共识(**Expected Consensus**)。参与选举的各方不需要给彼此之间投票，每个节点只需要自己内部计算，如果满足某个结果，就赢得了选举，拥有区块打包权。因此在每一轮选举过程中会选举一名或者多名矿工来创建新的block，也有可能此轮一个获胜者都没有,那么此回合被称为空回合。如果有多个矿工赢得选举，那么此轮会有多个区块，这些区块组合被称为Tipset。


## Filecoin的EC共识机制的底层实现
上文介绍过启动Miner会有一个全局的Miner结构StorageMinerAPI，里面有两个成员**storage.Miner**和**miner.Miner**，storage.Miner主要是负责数据存储密封，复制证明，时空证明这个过程，miner.Miner则负责挖矿出块相关。
![麦客存储](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/c5d622d7c7f64c33a8e1ba036777f5d1~tplv-k3u1fbpfcp-zoom-1.image)

其中api是与区块节点通信接口，epp提供根据挑战生成时空证明的接口，waitFunc是一个回调函数。当启动Miner服务时，会初始化一个miner.Miner结构，然后会在miner.Miner的start接口开一个协程执行mine函数。![麦客存储](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/2d6d4432287745f68967963a2881cb9b~tplv-k3u1fbpfcp-zoom-1.image)
这个函数比较长所以截图不全，trace.StartSpan是启动一个链路追踪的服务。mine会开一个无限循环，for 里面select主要是做退出控制。内部又有一个for循环:
![麦客存储](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/8bfbb87275ca4d1daa0453dc979db1ef~tplv-k3u1fbpfcp-zoom-1.image)
这个循环里面做了如下操作，GetBestMiningCandidate主要是获取最新区块的TipSet，然后会一直通过api调用区块节点的BeaconGetEntry接口，判断最新回合有没有开始，如果有开始则记录上回合的TipSet，退出循环以上一轮的TipSet为基础进行出块，每个挖矿节点需要等到新的回合才能开始操作

当新的回合启动后会调用mineOne开始竞选winner，只有winner才有出块权：
![麦客存储](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/95885868debc44ce8f2b8eb98f3e2c51~tplv-k3u1fbpfcp-zoom-1.image)
## 计算Ticket
首先做一些参数初始化工作和获取一些参与出块的必须参数，然后会根据这些参数调用computeTicket计算Ticket(ps:区块生成每回合都要生成一个Ticket)，我们看一下Ticket是怎么生成的:
![麦客存储](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/38bebb62eab54339b782cf4ac8facc4f~tplv-k3u1fbpfcp-zoom-1.image)
Ticket的计算依赖一个随机数，需要了解随机数的实现
​
### 计算随机数
随机数的计算过程如下:
![麦客存储](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/16893ff6d0ec4980999361715a2dcf27~tplv-k3u1fbpfcp-zoom-1.image)
随机数的计算依赖一些参数,rebase是上一回合区块中被称为**BeaconEntry**的结构,详细可以看[Filecoin随机数产生](https://beta.spec.filecoin.io/algorithms/crypto/randomness/) ，pers是一个枚举类型，表示随机数类型，round表示回合，entropy是miner的地址序列化的结果
![](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/d0596292164f4799bd634375932063c5~tplv-k3u1fbpfcp-zoom-1.image)
根据文章描述，可以Round记录回合数，Data表示该回合产生的随机数签名。下一轮随机数产生需要使用上一轮的随机数签名结合Miner节点地址等参数做blake2b-256运算，结果产生一个256byte的签名。
​
随机数生成后需要调用ComputeVRF函数，这里牵扯到Filecoin里面VRF的概念
### 什么是VRF
VRF(Verifiable Random Function)可验证的随机函数，详情请看 [VRF原理](https://filecoin-1.gitbook.io/spec/src/algorithms/crypto/vrf)
​
VRF类似钱包签名的概念，钱包私钥可以对任意消息进行签名获取摘要，而公钥可以验证签名 
​
` Any third party in possession of the corresponding public key, the message, and the VRF output, can verify if the digest has been computed correctly and by the correct signer` 
​
根据释义可以看出公钥的拥有者，消息，和VRF输出都可以去验证这个摘要的正确性。
​
在Ticket的生成过程中，通过VRF的方式可以允许任何人去验证这个产生的区块是否来自符合条件的生产者。即如果某个矿工成功赢得选举提交区块，其他矿工可以通过提交的VRF输出判断这个矿工winner是否符合条件
​
## 如何选出 Winner
选举winner调用的是IsRoundWinner函数
![麦客存储](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/d9648c9dedfd408ba7a5c55b3e20b958~tplv-k3u1fbpfcp-zoom-1.image)
​
也会有一套生成可验证签名的过程，最终会调用ComputeWinCount，传入参数是当前Miner算力和当前网络算力:
![麦客存储](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/7f5c0ecf49ed46fdb06170008171c1b6~tplv-k3u1fbpfcp-zoom-1.image)
算法貌似和泊松分布有关系，根据算法的原理可以了解到Miner的算力占比越大赢得选举的可能性就越大。
​
## 计算时空证明
当成功当选为Winner后，Miner需要产出一个有效的PoSt证明(时空证明),证明它们确实提供了存储空间，提供的时空证明需要参加领导者选举（因为可能会有多个矿工成为winner获取区块打包权），矿工存储能力越强，那么获胜的几率就越大，矿工根据它们提交时空证明证明的存储空间的大小获取奖励，这说明**如果Miner在这期间提交的证明越多，那么它获取的奖励也就越多**，在选举的过程中，还包含了一个**Surprise PoSt**机制，这个机制会随机的选取一些矿工要这些矿工提交时空证明去证明自己的存储空间，如果没有应答，那么就会掉算力，并且抵押的FIL也会被没收，如果连续两次没有应答，就会被系统清除矿工信息。
​
所以，没有参与选举的矿工，如果被Surprise PoSt机制抽中，也需要提交自己的时空证明。这种机制还有一个好处就是如果一些新参与的矿工，刚开始没有打包区块的权利，可以通过这种机制提交时空证明获取算力。
![麦客存储](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/365937e3c444484d9ec9ae5b4e9ff629~tplv-k3u1fbpfcp-zoom-1.image)
这里会调用Rust中的的函数，根据SectorInfo数据生成时空证明的结果
​
## 消息池
当生成证明之后，需要从区块节点的消息池选出一些message打包到区块中，去提交到链上
![麦客存储](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/a3f76100df024a108bf54520582fca7d~tplv-k3u1fbpfcp-zoom-1.image)
​
## 生成区块
当从消息池获取到message后，开始创建区块：
![麦客存储](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/a307a6769afa438b89725591dd69f600~tplv-k3u1fbpfcp-zoom-1.image)
Miner的地址信息，证明信息都会被打包到区块头中，最终提交到链上进行广播，经过多数节点确认后此轮挖矿回合结束。