#  Merkle Tree RFC 6919



- 完成了10W叶子节点的Merkle tree的搭建
- 完成了inclusion proof及verify
- 完成了exclusion proof及verify

## 文件说明

#### node.h node.cpp 

节点类及相关函数的实现

#### tree.h tree.cpp

merkle tree类，inclusion proof及exclusion proof和相关Verify

#### sm3.h sm3.cpp

自主实现的SM3 hash

## Merkle tree 搭建



根据RFC6919，通过分析，叶子节点的不同可以分成三类不同的情况

- 叶子结点为奇数
- 叶子结点偶数但不为2的幂
- 叶子结点为2的幂



- 当叶子结点为奇数时，将落单叶子结点放在第一个满足该层节点加上该节点为2的次幂时
- 当叶子结点偶数但不为2的幂，最底层放置不大于叶子节点数最大的2次幂个节点，将剩余的节点暂存，当满足该层节点加上该节点为2的次幂时，将剩余节点放置到该层
- 当叶子结点为2的幂，直接两两hash如此循环得到root

![image-20220731015052701](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220731015052701.png)

## inclusion proof

Merkle Trees除了不可篡改性起重要作用，也提供了SPV（Simplified Payment Verification）节点即使不用全部下载交易信息也能验证交易的方案。

简单地来说 SPV 节点并不会下载全部的区块链数据，而只是下载块头，这条链并不包括交易数据. 当前的某个时刻比特币区块链的高度是 550387, 一个区块头的大小是 80 个字节, 那么由区块头组成的链的大小大概是 42 M, 这和整个区块链动辄上几百 G 的数据相比少了很多很多。

交易验证而言，SPV节点不会所有交易都去验证，它只对没有验证的与它相关的交易进行验证，于是SPV节点会设置一个filter只去接受匹配它设置的fliter的交易。比如：
SPV接收到一笔钱，并不能确认这个交易是否合法，因此要对这个交易的输入进行验证。SPV要拿着这个交易的信息向网络发起查询请求getdata，这个请求的响应被称为merkle block message。
当有全节点收到这个请求之后，利用传过来的交易信息在自己的区块链数据库中进行查询，并把验证路径返回给请求源，SPV节点拿到验证路径之后，再做一次merkle校验，

对于存在性证明，即找到该叶子结点的审核路径。将兄弟节点和父节点的兄弟节点放入证明中

![image-20220731020006212](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220731020006212.png)

验证则是通过两两Hash得到一个新的ROOT,然后与ROOT比较，若相等则判断成功，在验证中还涉及到节点为左节点还是右节点的问题，以此来决定Hash的顺序

通过Checkdir()来实现，即判断该节点为左节点还是右节点。

![image-20220731020330502](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220731020330502.png)

## exclusion proof

首先我们要实现sorted tree，即将节点进行排序。

我们找到待证明节点的相邻节点（不等于待证明节点）

通过两点来判断不存在性

- 两节点需为待证明节点的相邻节点，若待证明节点在Merkle tree中，则这两个节点不相邻
- 找到这两个节点后，生成它们的存在性证明



若这两个节点相邻且这两个节点都存在Merkle tree中，则证明待证明节点不在Merkle Tree中









## 运行演示

