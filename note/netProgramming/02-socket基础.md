# table of contents

- [socket](#socket)
- [字节序](#字节序)
  - [字节序转换](#字节序转换)
- [socket地址结构](#socket地址结构)
  - [通用socket地址结构](#通用socket地址结构)
  - [专用socket地址结构](#专用socket地址结构)
- [IP地址转换函数](#ip地址转换函数)

## [socket](#table-of-contents)

```text
socket（套接字）是对网络中不同主机上的应用程序之间进行双向通信的端点的抽象，是一种特殊的文件描述符，可以用来实现不同主机间的进程通信或者同一主机上不同进程间的通信。

网络套接字上联应用程序进程下联网络协议栈。可以通过套接字接口向套接字写入数据或从套接字读取数据。套接字接口是对传输层协议的封装，应用程序通过套接字接口来访问传输层协议。

在 linux 系统中，socket 是一种文件描述符，其本质为内核借助缓冲区形成的伪文件。socket 也具有文件的特性，可以对其进行读写操作，但是 socket 不能用 lseek() 函数进行定位，也不能用 mmap() 函数将其映射到内存空间。socket 也有一些自己的操作函数，如 socket()、bind()、listen()、connect()、accept()、send()、recv() 等，这些函数与文件操作函数类似。
```

## [字节序](#table-of-contents)

```text
字节序是指在多字节数据在内存中的存放顺序。在计算机系统中，数据的存储单元是字节，每个字节都有其对应的地址。对于多字节的数据，在内存中存储时，可以按照从低地址到高地址存放，也可以按照从高地址到低地址存放。如果将高位字节存储在低地址，低位字节存储在高地址，则称为大端字节序（big-endian）；如果将低位字节存储在低地址，高位字节存储在高地址，则称为小端字节序（little-endian）。

在网络中，不同的计算机可能具有不同的字节序，因此在进行网络通信时，需要统一字节序。网络字节序采用大端字节序，即高位字节存储在低地址，低位字节存储在高地址。
```

### [字节序转换](#table-of-contents)

```text
字节序转换是指将主机字节序转换为网络字节序，或将网络字节序转换为主机字节序。字节序转换函数包括 htons()、htonl()、ntohs()、ntohl()。

- h : host 主机，主机字节序
- n : network 网络，网络字节序
- s : short 短整型
- l : long 长整型
```

> [字节序转换函数](/code/04-linux网络编程/31-socket基础/bytetrans.c)

```c
#include <arpa/inet.h>

// 转换端口
uint16_t htons(uint16_t hostport); // host to network short
uint16_t ntohs(uint16_t netport);  // network to host short

// 转换 IP 地址
uint32_t htonl(uint32_t hostaddr); // host to network long
uint32_t ntohl(uint32_t netaddr);  // network to host long
```

## [socket地址结构](#table-of-contents)

### [通用socket地址结构](#table-of-contents)

```c
#include <sys/socket.h>

struct sockaddr {
    sa_family_t sa_family; // 地址族
    char sa_data[14];      // 14 字节的协议地址
};
```

> 其中，sa_family 为地址族（sa_family_t）类型的变量，用于指定 socket 地址结构中的地址族。地址族通常与协议族（protocol family 也称 domain）对应，关系如下：

| 地址族 | 协议族 | 说明 |
| :--: | :--: | :--: |
| PF_UNIX | AF_UNIX | UNIX本地域协议族 |
| PF_INET | AF_INET | TCP/IPv4协议族 |
| PF_INET6 | AF_INET6 | TCP/IPv6协议族 |

> sa_data 成员用于存放 socket 地址值，不同的协议有不同的含义和长度，如下：

| 协议族 | 地址值含义和长度 |
| :-- | :-- |
| PF_UNIX | 文件的路径名，长度可达到108字节 |
| PF_INET | 16 bit 端口号和 32 bit IPv4 地址，共 6 字节 |
| PF_INET6 | 16 bit 端口号，32 bit 流标识，128 bit IPv6 地址，32 bit 范围 ID，共 26 字节 |

> 由上表可知，14 字节的 sa_data 根本无法容纳多数协议族的地址值。因此，Linux 定义了下面这个新的通用的 socket 地址结构体，这个结构体不仅提供了足够大的空间用于存放地址值，而且是内存对齐的。

```c
#include <bits/socket.h>

struct sockaddr_storage {
    sa_family_t sa_family;
    unsigned long int __ss_align;
    char __ss_padding[ 128 - sizeof(__ss_align) ];
};

typedef unsigned short int sa_family_t;
```

### [专用socket地址结构](#table-of-contents)

```text
很多网络编程函数诞生早于 IPv4 协议，那时候都使用的是 struct sockaddr 结构体，为了向前兼容，现在sockaddr 退化成了（void *）的作用，传递一个地址给函数，至于这个函数是 sockaddr_in 还是 sockaddr_in6，由地址族确定，然后函数内部再强制类型转化为所需的地址类型。

所有专用 socket 地址（以及 sockaddr_storage）类型的变量在实际使用时都需要转化为通用 socket 地址类型 sockaddr（强制转化即可），因为所有 socket 编程接口使用的地址参数类型都是 sockaddr。
```

> UNIX 本地域协议族使用如下专用的 socket 地址结构体：

```c
#include <sys/un.h>

struct sockaddr_un {
    sa_family_t sun_family; // 16位 地址类型
    char sun_path[108];     // 14字节 协议地址
};
```

> TCP/IPv4 协议族使用如下专用的 socket 地址结构体：

```c
#include <netinet/in.h>

struct sockaddr_in {
    sa_family_t sin_family; // 16位 地址类型
    uint16_t sin_port;      // 16位 端口号
    struct in_addr sin_addr;// 32位 IPv4 地址
    char sin_zero[8];       // 8字节 填充
};

struct in_addr {
    uint32_t s_addr;        // IPv4 地址
};
```

> TCP/IPv6 协议族使用如下专用的 socket 地址结构体：

```c
#include <netinet/in.h>

struct sockaddr_in6 {
    sa_family_t sin6_family;   // 16位 地址类型
    uint16_t sin6_port;        // 16位 端口号
    uint32_t sin6_flowinfo;    // 32位 流标识
    struct in6_addr sin6_addr; // 128位 IPv6 地址
    uint32_t sin6_scope_id;    // 32位 范围 ID
};

struct in6_addr {
    unsigned char s6_addr[16]; // IPv6 地址
};
```

## [IP地址转换函数](#table-of-contents)

> 通常，人们习惯用可读性好的字符串来表示 IP 地址，比如用点分十进制字符串表示 IPv4 地址，以及用十六进制字符串表示 IPv6 地址。但编程中我们需要先把它们转化为整数（二进制数）方能使用。而记录日志时则相反，我们要把整数表示的 IP 地址转化为可读的字符串。下面 3 个函数可用于用点分十进制字符串表示的 IPv4 地址和用网络字节序整数表示的 IPv4 地址之间的转换：

```c
#include <arpa/inet.h>

// 将点分十进制字符串转换为网络字节序的整数值
int inet_aton(const char *strptr, struct in_addr *addrptr);

// 将点分十进制字符串转换为网络字节序的整数值
in_addr_t inet_addr(const char *strptr);

// 将网络字节序的整数值转换为点分十进制字符串
char *inet_ntoa(struct in_addr inaddr);
```

> 下面这对[更新的函数](/code/04-linux网络编程/31-socket基础/iptrans.c)也能完成前面 3 个函数同样的功能，并且它们同时适用 IPv4 地址和 IPv6 地址：

```c
#include <arpa/inet.h>

// 将网络字节序的整数值转换为点分十进制字符串
const char *inet_ntop(int domain, const void *restrict addr, char *restrict str, socklen_t size);

// 将点分十进制字符串转换为网络字节序的整数值
int inet_pton(int domain, const char *restrict str, void *restrict addr);
```
