# table of contents

- [IPC](#ipc)
  - [conception](#conception)
  - [IPC的实现方式](#ipc的实现方式)
    - [同一主机间的进程通信](#同一主机间的进程通信)
    - [不同主机间的进程通信](#不同主机间的进程通信)
- [Pipe](#pipe)
  - [conception](#conception-1)
  - [function](#function)
- [FIFO](#fifo)
  - [conception](#conception-2)
  - [function](#function-1)
- [Message Queue](#message-queue)
- [Shared Memory](#shared-memory)
- [Signal](#signal)
  - [conception](#conception-3)
  - [linux signal](#linux-signal)
  - [function](#function-2)
  - [signal set](#signal-set)
- [memory-mapped](#memory-mapped)
  - [conception](#conception-4)
  - [function](#function-3)
  - [think](#think)

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

### [conception](#table-of-contents)

```text
有名管道（FIFO）不同于匿名管道之处在于它提供了一个路径名与之关联，以 FIFO 的文件形式存在于文件系统中，并且其打开方式与打开一个普通文件是一样的，这样即使与 FIFO 的创建进程不存在亲缘关系的进程，只要可以访问该路径，就能够彼此通过 FIFO 相互通信，因此，通过 FIFO 不相关的进程也能交换数据。

FIFO 也是一种半双工的通信方式，数据只能单向流动，需要双向通信时，需要建立起两个 FIFO。

FIFO 也是一种文件，它在文件系统中以一种特殊的方式存在，但它不是普通的文件，它不属于某种文件类型，它的文件类型是 FIFO，文件权限与创建它的进程的 umask 值有关。

FIFO 也是一种特殊的文件，它不占用磁盘空间，它的数据在内存中，它的数据只在内存中存在，不会写入到磁盘中。当使用 FIFO 文件的进程退出时，FIFO 文件不被删除，只有当所有使用 FIFO 文件的进程都退出时，FIFO 文件才会被删除。

FIFO 也是一种特殊的文件，它的创建不是由 open 函数来创建的，而是由 mkfifo 命令或函数来创建的，它的创建与打开是分开的，使用两个不同的函数，是两个不同的系统调用。
```

### [function](#table-of-contents)

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

### [conception](#table-of-contents)

```text
Signal 是 linux 系统 IPC 最古老的一种通信机制，它是一种异步通信机制，用于通知接收进程某个事件已经发生。

有时被称为软件中断，它是一种软件层次的中断，与硬件中断不同，它不是由外部事件产生的，而是由软件自身产生的。

引发信号的事件有很多种，比如：
对于前台进程，用户按了 Ctrl+C ，这时会产生一个 SIGINT 信号，进程收到这个信号后，会终止运行。
对于后台进程，用户按了 Ctrl+Z ，这时会产生一个 SIGTSTP 信号，进程收到这个信号后，会暂停运行。
硬件发生了某个异常，这时会产生一个 SIGSEGV 信号，进程收到这个信号后，会终止运行。
系统状态发生变化，这时会产生一个 SIGCHLD 信号，进程收到这个信号后，会处理子进程的退出状态。
运行 kill 命令，这时会产生一个 SIGTERM 信号，进程收到这个信号后，会终止运行。
```

### [linux signal](#table-of-contents)

> 可以使用 man 7 signal 查看信号的详细信息。
| 信号处理方式 | 说明 |
| :---: | :---: |
| TERM | 默认方式，终止进程 |
| IGN | 忽略信号 |
| CORE | 终止进程并生成 core 文件 |
| STOP | 暂停进程 |
| CONT | 恢复进程 |
| HAND | 由应用程序指定信号处理函数 |
>
> 可以使用 kill -l 命令查看 linux 系统支持的所有信号。其中前 31 个信号是 linux 系统默认的信号，后面的信号是 linux 系统扩展的信号。
>
> 常用到的信号：
>> SIGINT：2，终端中断信号，通常是 Ctrl+C 产生，用于通知前台进程组终止进程。
>>
>> SIGQUIT：3，终端退出信号，通常是 Ctrl+\ 产生，用于通知前台进程组终止进程，并生成 core 文件。
>>
>> SIGKILL：9，终止信号，用于强制终止进程，进程不能捕获和忽略该信号。
>>
>> SIGSTOP：19，停止信号，用于暂停进程，进程不能捕获和忽略该信号。
>>
>> SIGCONT：18，继续信号，用于恢复进程，进程不能捕获和忽略该信号。
>>
>> SIGSEGV：11，段错误信号，用于通知进程发生了段错误。
>>
>> SIGCHLD：17，子进程退出信号，用于通知父进程子进程已经退出。
>>
>> SIGALRM：14，定时器信号，用于通知进程定时器已经超时。
>>
>> SIGUSR1：10，用户自定义信号1，用于通知进程用户自定义事件1已经发生。
>>
>> SIGUSR2：12，用户自定义信号2，用于通知进程用户自定义事件2已经发生。
>

### [function](#table-of-contents)

```c
// 信号处理函数
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
/*
    功能：
        设置信号处理函数
    参数：
        signum：信号编号
        handler：信号处理函数
    返回值：
        成功：返回原来的信号处理函数
        失败：返回 SIG_ERR
*/
int segaction(int signum, const struct sigaction *act, struct sigaction *oldact);
/*
    功能：
        设置信号处理函数
    参数：
        signum：信号编号
        act：新的信号处理函数
        oldact：旧的信号处理函数
    返回值：
        成功：0
        失败：-1
*/

// 发送信号
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
/*
    功能：
        向指定进程发送信号
    参数：
        pid：进程ID
        sig：信号编号
    返回值：
        成功：0
        失败：-1
*/
int raise(int sig);
/*
    功能：
        向自身进程发送信号
    参数：
        sig：信号编号
    返回值：
        成功：0
        失败：-1
*/
void abort(void);
/*
    功能：
        终止进程
    参数：
        无
    返回值：
        无
*/
unsigned int alarm(unsigned int seconds);
/*
    功能：
        设置定时器
    参数：
        seconds：定时器超时时间
    返回值：
        成功：返回原来的定时器超时时间
        失败：返回0
*/
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
/*
    功能：
        设置定时器
    参数：
        which：定时器类型
            - ITIMER_REAL：真实时间定时器
            - ITIMER_VIRTUAL：用户态时间定时器
            - ITIMER_PROF：用户态和内核态时间定时器
        new_value：新的定时器超时时间
        old_value：旧的定时器超时时间
    返回值：
        成功：0
        失败：-1
*/
```

### [signal set](#table-of-contents)

```text
信号集是一个用来存放信号的集合，信号集中的每个元素都对应一个信号，信号集中的每个元素都有一个编号，这个编号就是信号的编号。

在 PCB 中有两个非常重要的信号集。一个称之为 “阻塞信号集” ，另一个称之为“未决信号集” 。
这两个信号集都是内核使用位图机制来实现的。
但操作系统不允许我们直接对这两个信号集进行位操作。而需自定义另外一个集合，借助信号集操作函数来对 PCB 中的这两个信号集进行修改。
```

```c
// 信号集操作函数
#include <signal.h>
int sigemptyset(sigset_t *set);
/*
    功能：
        清空信号集
    参数：
        set：信号集
    返回值：
        成功：0
        失败：-1
*/
int sigfillset(sigset_t *set);
/*
    功能：
        将所有信号添加到信号集中
    参数：
        set：信号集
    返回值：
        成功：0
        失败：-1
*/
int sigaddset(sigset_t *set, int signum);
/*
    功能：
        将指定信号添加到信号集中
    参数：
        set：信号集
        signum：信号编号
    返回值：
        成功：0
        失败：-1
*/
int sigdelset(sigset_t *set, int signum);
/*
    功能：
        将指定信号从信号集中删除
    参数：
        set：信号集
        signum：信号编号
    返回值：
        成功：0
        失败：-1
*/
int sigismember(const sigset_t *set, int signum);
/*
    功能：
        判断指定信号是否在信号集中
    参数：
        set：信号集
        signum：信号编号
    返回值：
        成功：1
        失败：0
*/
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
/*
    功能：
        用自定义的信号集来设置阻塞信号集
    参数：
        how：操作方式
            - SIG_BLOCK：将 set 指向的信号集中的信号添加到阻塞信号集中
            - SIG_UNBLOCK：将 set 指向的信号集中的信号从阻塞信号集中删除
            - SIG_SETMASK：将 set 指向的信号集中的信号设置为阻塞信号集
        set：信号集
        oldset：旧的阻塞信号集
    返回值：
        成功：0
        失败：-1
*/
int sigpending(sigset_t *set);
/*
    功能：
        获取未决信号集
    参数：
        set：未决信号集
    返回值：
        成功：0
        失败：-1
*/
```

## [memory-mapped](#table-of-contents)

### [conception](#table-of-contents)

```text
内存映射技术是一种将文件映射到进程的内存空间里，使得进程可以像访问内存一样访问文件的技术。进程通信是指在不同的进程之间交换信息的技术。 内存映射和进程通信是两种不同的技术，但它们之间有一定的关系:

一方面，内存映射可以被用来实现进程间通信。通过将一个文件或共享内存映射到不同的进程的地址空间中，这些进程可以直接访问相同的内容。这样可以避免使用传统的IPC方法如管道或共享内存的复杂性，提高进程间通信的效率。

另一方面，进程通信也可以利用内存映射技术。例如，可以将一段内存映射到一个共享文件，然后将该文件的句柄通过进程通信的方式发送给另一个进程。另一个进程可以通过相同的方式将该文件映射到其自己的地址空间中，并访问相同的数据。

因此，可以将内存映射和进程通信结合起来使用，以实现更高效和便捷的数据共享。不过需要注意，内存映射技术在使用时应该遵循相关的安全和同步规范，以保证进程之间访问数据的正确性和一致性。
```

### [function](#table-of-contents)

```c
/*
    #include <sys/mman.h>
    void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
        - 功能：将一个文件或者设备的数据映射到内存中
        - 参数：
            - void *addr: NULL, 由内核指定
            - length : 要映射的数据的长度，这个值不能为0。建议使用文件的长度。
                    获取文件的长度：stat lseek
            - prot : 对申请的内存映射区的操作权限
                -PROT_EXEC ：可执行的权限
                -PROT_READ ：读权限
                -PROT_WRITE ：写权限
                -PROT_NONE ：没有权限
                要操作映射内存，必须要有读的权限。
                PROT_READ、PROT_READ|PROT_WRITE
            - flags :
                - MAP_SHARED : 映射区的数据会自动和磁盘文件进行同步，进程间通信，必须要设置这个选项
                - MAP_PRIVATE ：不同步，内存映射区的数据改变了，对原来的文件不会修改，会重新创建一个新的文件。（copy on write）
            - fd: 需要映射的那个文件的文件描述符
                - 通过open得到，open的是一个磁盘文件
                - 注意：文件的大小不能为0，open指定的权限不能和prot参数有冲突。
                    prot: PROT_READ                open:只读/读写 
                    prot: PROT_READ | PROT_WRITE   open:读写
            - offset：偏移量，一般不用。必须指定的是4k的整数倍，0表示不便宜。
        - 返回值：返回创建的内存的首地址
            失败返回MAP_FAILED，(void *) -1

    int munmap(void *addr, size_t length);
        - 功能：释放内存映射
        - 参数：
            - addr : 要释放的内存的首地址
            - length : 要释放的内存的大小，要和mmap函数中的length参数的值一样。
*/

/*
    使用内存映射实现进程间通信：
    1.有关系的进程（父子进程）
        - 还没有子进程的时候
            - 通过唯一的父进程，先创建内存映射区
        - 有了内存映射区以后，创建子进程
        - 父子进程共享创建的内存映射区
    
    2.没有关系的进程间通信
        - 准备一个大小不是0的磁盘文件
        - 进程1 通过磁盘文件创建内存映射区
            - 得到一个操作这块内存的指针
        - 进程2 通过磁盘文件创建内存映射区
            - 得到一个操作这块内存的指针
        - 使用内存映射区通信

    注意：内存映射区通信，是非阻塞。
*/
```

### [think](#table-of-contents)

> 如果对mmap的返回值(ptr)做++操作(ptr++), munmap是否能够成功？
>> 不能，因为munmap的参数是void\* 类型，而 ptr++ 是 int\* 类型，两者类型不一致。
>
> 如果open时O_RDONLY, mmap时prot参数指定PROT_READ | PROT_WRITE会怎样？
>> open时O_RDONLY，mmap时prot参数指定PROT_READ | PROT_WRITE会失败，因为prot参数指定的权限和open时指定的权限冲突。
>
> 如果文件偏移量为1000会怎样？
>> 会成功，但是映射区的大小会是文件大小加上偏移量，即映射区的大小为文件大小加上偏移量，而不是文件大小减去偏移量。
>
> mmap什么情况下会调用失败？
>>
>> 1. 文件大小为0
>> 2. open时指定的权限和prot参数指定的权限冲突
>> 3. 文件大小加上偏移量不是4k的整数倍
>
> 可以open的时候O_CREAT一个新文件来创建映射区吗？
>> 可以，但是文件大小不能为0，否则mmap会失败。
>
> mmap后关闭文件描述符，对mmap映射有没有影响？
>> 没有影响，因为mmap映射的是文件的内容，而不是文件描述符。
>
> 对ptr越界操作会怎样？
>> 会导致段错误。
>
> munmap后，ptr还能使用吗？
>> 不能，因为munmap后，ptr指向的内存已经被释放了。
>