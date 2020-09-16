# quic协议详解

quic(Quick UDP Internet Connection) 是基于udp 的 多路复用流传输协议，由谷歌开发。相比于传统TCP，它减少了传输延时。它是实现在udp 上的 TCP+TLS+HTTP/2。

## quic 相比TCP + TLS + HTTP/2 有哪些特性

- 极大的减少了建连时间

- 改进的拥塞控制算法

- 避免队头阻塞的多路复用

- 前馈错误纠正

- 连接迁移

## 建立连接

Quic 的握手差不多都是0 rtt 的，而 tcp + tls 需要 1~3个rtt。 对于client 与 server 的第一次建立连接，是需要1 rtt 去获取相关信息来完成握手的。 client 发送一个空的 hello 包(CHLO)，  server发送一个REJ包，在这个REJ包中，包含有client所需要的一些信息。包括token 和 server 的证书。下次client再发送 CHLO包的时候，可以使用缓存的token与证书发送加密的请求给server

## 拥塞控制

Quic使用可插拔的拥塞控制，相较于TCP，它能提供更丰富的拥塞控制信息。比如对于每一个包，不管是原始包还是重传包，都带有一个新的序列号(seq)，这使得Quic能够区分ACK是重传包还是原始包，从而避免了TCP重传模糊的问题。Quic同时还带有收到数据包与发出ACK之间的时延信息。这些信息能够帮助更精确的计算rtt。此外，Quic的ACK Frame 支持256个NACK 区间，相比于TCP的SACK 更弹性化，更丰富的信息会让client和server 哪些包已经被对方收到。

## 多路复用

基于TCP的HTTP2最大的一个问题就是对头阻塞。应用程序视TCP连接为一个字节流，当一个TCP包丢失的时候，HTTP2上的任意一条字节流都不能被后续处理，除非收到重传包。由于Quic 被设计用于多路复用，所以某条流上丢失一个包，只会影响这一条流，其它未丢失数据的流可以继续进行后续的处理。

## 前馈错误纠正

为了恢复丢失的包，而不需要等待重传。Quic会给一组包补充一个FEC包，该FEC包 包含了 这一组包的奇偶性信息，如果这一组包中的某个包丢失了，该包的内容可以通过FEC包和其它的包进行恢复。发送发(sender)可以决定是否发送FEC包来进行一些特殊场景的优化。

## 连接迁移

Quic 的连接标识是一个 64bit 的 connection ID, 由client 随机产生。而TCP连接由四元组标识(源ip,源port, 目的ip, 目的port)，这意味着，如果client 的ip(比如wifi断了) 或者 port 变化了，任何活跃的TCP连接都不在有效。而当一个Quic client 的ip 变化的时候，它可以继续使用原来的connection ID 而不会中断连接。