# table of contents

## [IPC](#table-of-contents)

### [conception](#table-of-contents)

> IPC: Inter-Process Communication

```text
进程是一个独立的资源分配单元，不同进程（这里所说的进程通常指的是用户进程）之间的资源是独立的，没有关联，不能在一个进程中直接访问另一个进程的资源。

但是，进程不是孤立的，不同的进程需要进行信息的交互和状态的传递等，因此需要进程
间通信( IPC：Inter Processes Communication )。

IPC目的：
1. 数据传输：一个进程需要将它的数据发送给另一个进程，另一个进程也需要接收这个数据。
2. 共享信息：多个进程需要共享一个公共的数据，比如说共享内存。
3. 通知事件：一个进程需要向另一个或多个进程发送消息，通知它们发生了某种事件。
4. 资源共享：多个进程需要共享同样的资源，比如说打印机。
5. 进程控制：一个进程需要控制另一个进程，比如说父进程控制子进程。
```

### [IPC的实现方式](#table-of-contents)

#### [同一主机间的进程通信](#table-of-contents)

```text
1. 管道（Pipe）：管道是半双工的，数据只能向一个方向流动，需要双方通信时，需要建立起两个管道。

2. 命名管道（FIFO）：命名管道克服了管道没有名字的限制，可以在无关的进程之间交换数据。

3. 信号（Signal）：信号是比较复杂的通信方式，用于通知接收进程某个事件已经发生。

4. 消息队列（Message Queue）：消息队列是消息的链表，存放在内核中并由消息队列标识符标识。

5. 共享内存（Shared Memory）：共享内存就是映射一段能被其他进程所访问的内存，这段共享内存由一个进程创建，但多个进程都可以访问。

6. 信号量（Semaphore）：信号量是一个计数器，可以用来控制多个进程对共享资源的访问。
```

#### [不同主机间的进程通信](#table-of-contents)

```text
1. 套接字（Socket）：套接字也是一种进程间通信机制，与其他通信机制不同的是，它可用于不同主机间的进程通信。
```

## [Pipe](#table-of-contents)

### [conception](#table-of-contents)

```text
管道也叫无名（匿名）管道，它是是 UNIX 系统 IPC（进程间通信）的最古老形式，所有的 UNIX 系统都支持这种通信机制。

管道是半双工的，数据只能向一个方向流动，需要双方通信时，需要建立起两个管道。

管道是内核内存维护的一段缓冲区，它具有一定的长度，它有一个读指针和一个写指针，读写指针都是指向缓冲区中的某一位置，它们的差值表示管道中的数据长度。
具体长度可以通过 fcntl(fd, F_GETPIPE_SZ) 或 ulimit -p 查看。

因为管道的实现机制是基于文件系统的，所以只能用于具有公共祖先的进程间通信。
```

### [function](#table-of-contents)

```c
// 创建管道
#include <unistd.h>
int pipe(int fd[2]);
/*
    功能：
        创建一个管道
    参数：
        fd[2]：用于返回管道的读写文件描述符，fd[0]用于读，fd[1]用于写
    返回值：
        成功：0
        失败：-1
*/

// 读写管道
// 读写管道的函数与读写文件的函数一样
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
/*
    功能：
        读写管道
    参数：
        fd：管道的文件描述符
        buf：用于存放读写数据的缓冲区
        count：要读写的字节数
    返回值：
        成功：读写的字节数
        失败：-1
*/
```

## [FIFO](#table-of-contents)

```text
有名管道（FIFO）不同于匿名管道之处在于它提供了一个路径名与之关联，以 FIFO 的文件形式存在于文件系统中，并且其打开方式与打开一个普通文件是一样的，这样即使与 FIFO 的创建进程不存在亲缘关系的进程，只要可以访问该路径，就能够彼此通过 FIFO 相互通信，因此，通过 FIFO 不相关的进程也能交换数据。

FIFO 也是一种半双工的通信方式，数据只能单向流动，需要双向通信时，需要建立起两个 FIFO。

FIFO 也是一种文件，它在文件系统中以一种特殊的方式存在，但它不是普通的文件，它不属于某种文件类型，它的文件类型是 FIFO，文件权限与创建它的进程的 umask 值有关。

FIFO 也是一种特殊的文件，它不占用磁盘空间，它的数据在内存中，它的数据只在内存中存在，不会写入到磁盘中。当使用 FIFO 文件的进程退出时，FIFO 文件不被删除，只有当所有使用 FIFO 文件的进程都退出时，FIFO 文件才会被删除。

FIFO 也是一种特殊的文件，它的创建不是由 open 函数来创建的，而是由 mkfifo 命令或函数来创建的，它的创建与打开是分开的，使用两个不同的函数，是两个不同的系统调用。
```

```c
// 创建FIFO
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
/*
    功能：
        创建一个FIFO文件
    参数：
        pathname：FIFO文件的路径名
        mode：FIFO文件的权限
    返回值：
        成功：0
        失败：-1
*/

// 打开FIFO
// 打开FIFO的函数与打开普通文件的函数一样
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
/*
    功能：
        打开一个FIFO文件
    参数：
        pathname：FIFO文件的路径名
        flags：打开方式
    返回值：
        成功：文件描述符
        失败：-1
*/
```

## [Message Queue](#table-of-contents)

```text

```

## [Shared Memory](#table-of-contents)

```text

```

## [Signal](#table-of-contents)

```text

```