# table of contents

- [UDP通信](#udp通信)
  - [UDP通信特点](#udp通信特点)
  - [UDP通信流程](#udp通信流程)
- [单播](#单播)
  - [单播通信](#单播通信)
- [广播](#广播)
  - [广播通信](#广播通信)
- [组播](#组播)
  - [组播通信](#组播通信)
  - [组播地址](#组播地址)
  
## [UDP通信](#table-of-contents)

### [UDP通信特点](#table-of-contents)

- 将数据及源和目的封装成数据包中，不需要建立连接
- 每个数据报的大小限制在64K内
- 因无连接，是不可靠协议
- 不需要建立连接，速度快

### [UDP通信流程](#table-of-contents)

> ![UDP通信流程](./images/UDP通信流程.png)

- 发送端
  - 建立发送端的Socket对象
  - 创建数据，并打包
  - 调用Socket对象的发送方法发送数据包
  - 关闭Socket对象

- 接收端
  - 建立接收端的Socket对象
  - 创建一个数据包，用于接收数据
  - 调用Socket对象的接收方法接收数据包
  - 解析数据包，并显示在控制台

## [单播](#table-of-contents)

```text
单播：一对一通信
```

### [单播通信](#table-of-contents)

> [单播通信S](/code/04-linux网络编程/37-UDP通信/udp_server.c)
> [单播通信C](/code/04-linux网络编程/37-UDP通信/udp_client.c)

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
作用：向指定的目的IP地址发送数据
参数：
    sockfd：用于传输数据的套接字
    buf：要发送的数据
    len：要发送的数据的长度
    flags：0
    dest_addr：目的IP地址
    addrlen：目的IP地址的长度
返回值：成功返回发送的数据的长度，失败返回-1

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
作用：接收数据
参数：
    sockfd：用于传输数据的套接字
    buf：用于接收数据的缓冲区
    len：缓冲区的长度
    flags：0
    src_addr：发送端的IP地址
    addrlen：发送端的IP地址的长度
返回值：成功返回接收到的数据的长度，失败返回-1
```

## [广播](#table-of-contents)

```text
广播：一对多通信。向同一网段内的所有主机发送数据。每个网段的最后一个IP地址为广播地址。
1. 只能在同一网段内广播
2. 客户端想要绑定服务器广播使用的端口
3. 服务器需要设置广播属性
```

### [广播通信](#table-of-contents)

> [广播通信S](/code/04-linux网络编程/37-UDP通信/bro_server.c)
> [广播通信C](/code/04-linux网络编程/37-UDP通信/bro_client.c)

```c
#include <sys/types.h>
#include <sys/socket.h>

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
作用：设置套接字的属性
参数：
    sockfd：要设置的套接字
    level：要设置的属性的级别 SOL_SOCKET
    optname：要设置的属性的名称 SO_BROADCAST
    optval：要设置的属性的值 1 0
    optlen：要设置的属性的值的长度 sizeof(optval)
返回值：成功返回0，失败返回-1

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
作用：获取套接字的属性
参数：
    sockfd：要获取的套接字
    level：要获取的属性的级别
    optname：要获取的属性的名称
    optval：要获取的属性的值
    optlen：要获取的属性的值的长度
返回值：成功返回0，失败返回-1
```

## [组播](#table-of-contents)

```text
组播：一对多通信。向同一组内的所有主机发送数据。组播地址为D类IP地址。
1. 可以跨网段
2. 客户端需要加入组播组
3. 服务器需要设置组播属性
```

### [组播通信](#table-of-contents)

> [组播通信S](/code/04-linux网络编程/37-UDP通信/multi_server.c)
> [组播通信C](/code/04-linux网络编程/37-UDP通信/multi_client.c)

```c
#include <sys/types.h>
#include <sys/socket.h>

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

// 服务器设置多播
level：IPPROTO_IP
optname：IP_MULTICAST_IF
optval：组播地址结构体 struct in_addr

// 客户端加入组播组
level：IPPROTO_IP
optname：IP_ADD_MEMBERSHIP
optval：组播地址结构体 struct ip_mreqn

typedef uint32_t in_addr_t;
struct in_addr {
    in_addr_t s_addr;
};

struct ip_mreqn {
    struct in_addr imr_multiaddr;   // 组播地址
    struct in_addr imr_address;     // 本机IP地址
    int            imr_ifindex;     // 网卡编号
};
```

### [组播地址](#table-of-contents)

```text
IP 多播通信必须依赖于 IP 多播地址，在 IPv4 中它的范围从 224.0.0.0 到 239.255.255.255 ,并被划分为局部链接多播地址、预留多播地址和管理权限多播地址三类:
```

| 类型 | 地址范围 | 用途 |
| :--- | :--- | :--- |
| 局部链接多播地址 | 224.0.0.0~224.0.0.255 | 是为路由协议和其它用途保留的地址，路由器并不转发属于此范围的IP包 |
| 预留多播地址 | 224.0.1.0~224.0.1.255 | 公用组播地址，可用于Internet；使用前需要申请 |
| 预留多播地址 | 224.0.2.0~238.255.255.255 | 用户可用组播地址(临时组地址)，全网范围内有效 |
| 本地管理组播地址 | 239.0.0.0~239.255.255.255 | 可供组织内部使用，类似于私有 IP 地址，不能用于 Internet，可限制多播范围 |
