# 内网穿透

## NAT概念

网络地址转换(Network Address Translation,缩写为NAT)，也叫做网络掩蔽或者IP掩蔽，是一种IP数据包通过路由器或防火墙时重写来源IP地址或目的IP地址的技术。NAT式作为一种IPv4地址短缺以保留IP地址困难方案流行起来的

## NAPT(网络地址端口转换)
NAPT是NAT的一种工作模式，这种方式支持端口的映射，并允许多个主机共享一个公网IP地址。支持端口转换的NAT又可以分为两类:
- 源地址转换：

    发起连接的计算机IP地址会被重写。使得内网主机发出的数据包能够到达外网主机。
  
  
- 目的地址转换:

    连接计算机的IP地址会被重写，使得外网发出的数据包能够到达内网主机。
    

以上两种方式通常会被一起使用支持双向通信，NAPT维护一个带有IP和端口号的NAT表。

| 内网IP&Port | 外网IP&Port | 



## ICE


## STUN
    NAT 的UDP简单穿越（Simple Traversal of UDP over NATs）是一种网络协议，它允许位于NAT（或多重NAT）后的客户端找出自己的公网地址，查出自己位于哪种类型的NAT之后以及NAT为某一个本地端口所绑定的Internet端端口。


## TURN

    中继NAT实现的穿透（Traversal Using Relays around NAT）就是透过TURN服务器开启连线并转送所有数据，进而绕过Symmetric NAT的限制。你可透过TURN服务器建立连线，再告知所有端点传送封包至该服务器，最后让服务器转送封包给你。这个方法更耗时且更占频宽，因此在没有其他替代方案时才会使用这个方法。