# 分布式存储ceph原理解析

ceph特点：
- 高性能
    -  摒弃了传统的集中式存储元数据寻址的方案，采用CRUSH算法，数据分布均衡，并行度高。
    - 考虑了容灾域的隔离，能够实现各类负载的副本放置规则，例如跨机房、机架感知等。
    - 能够支持上千个存储节点的规模，支持TB到PB级的数据。

- 高可用
    - 副本数可以灵活控制。
    - 支持故障域分隔，数据强一致性。
    - 多种故障场景自动进行修复自愈。
    - 没有单点故障，自动管理。
- 高扩展性
    - 去中心化。
    - 扩展灵活。
    - 随着节点增加而线性增长。
- 特性丰富
    - 支持三种存储接口：块存储、文件存储、对象存储。
    - 支持自定义接口，支持多种语言驱动。



## 核心组件

- Monitor

    一个 Ceph 集群需要多个 Monitor 组成的小集群，它们通过 Paxos 同步数据，用来保存 OSD 的元数据。

- MDS 

    MDS 全称 Ceph Metadata Server，是 CephFS 服务依赖的元数据服务。

- Object

    Ceph 最底层的存储单元是 Object 对象，每个 Object 包含元数据和原始数据。

- PG 

    PG 全称 Placement Grouops，是一个逻辑的概念，一个 PG 包含多个 OSD。引入 PG 这一层其实是为了更好的分配数据和定位数据。

- RADOS

    RADOS 全称 Reliable Autonomic Distributed Object Store，是 Ceph 集群的精华，用户实现数据分配、Failover 等集群操作。

- Libradio

    Librados 是 Rados 提供库，因为 RADOS 是协议很难直接访问，因此上层的 RBD、RGW 和 CephFS 都是通过 librados 访问的，目前提供 PHP、Ruby、Java、Python、C 和 C++ 支持。

- CRUSH

    CRUSH 是 Ceph 使用的数据分布算法，类似一致性哈希，让数据分配到预期的地方。

- RBD

    RBD 全称 RADOS block device，是 Ceph 对外提供的块设备服务。

- RGW

    RGW 全称 RADOS gateway，是 Ceph 对外提供的对象存储服务，接口与 S3 和 Swift 兼容。

- CephFS

    CephFS 全称 Ceph File System，是 Ceph 对外提供的文件系统服务。