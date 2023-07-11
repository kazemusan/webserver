#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"

#define MAX_FD 65536           // 最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000 // 监听的最大的事件数量

// 添加文件描述符
extern void addfd(int epollfd, int fd, bool one_shot);
extern void removefd(int epollfd, int fd);

// 添加信号处理函数
void addsig(int sig, void(handler)(int))
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

int main(int argc, char *argv[])
{
    // 参数检查
    if (argc <= 1)
    {
        printf("usage: %s port_number\n", basename(argv[0]));
        return 1;
    }

    // 忽略SIGPIPE信号,防止因为客户端关闭连接导致服务器崩溃
    addsig(SIGPIPE, SIG_IGN);

    // 创建线程池
    threadpool<http_conn> *pool = NULL;
    try
    {
        pool = new threadpool<http_conn>;
    }
    catch (...)
    {
        return 1;
    }

    // 预先为每个可能的客户连接分配一个http_conn对象
    http_conn *users = new http_conn[MAX_FD];

    // 设置服务器的IP地址和端口号
    int port = atoi(argv[1]);
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // 创建监听socket
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    // 设置端口复用
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // 绑定端口,监听
    int ret = 0;
    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    ret = listen(listenfd, 5);

    // 创建epoll对象,并将监听socket添加到epoll对象中
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    addfd(epollfd, listenfd, false);

    // 设置http_conn的静态变量
    http_conn::m_epollfd = epollfd;

    // 循环监听,处理客户连接
    while (true)
    {
        // 阻塞监听事件,直到有事件发生，并返回事件数量
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);

        // 无事件发生但被系统中断
        if ((number < 0) && (errno != EINTR))
        {
            printf("epoll failure\n");
            break;
        }

        // 循环处理事件
        for (int i = 0; i < number; i++)
        {
            // 获取事件fd
            int sockfd = events[i].data.fd;

            // 正在监听的事件
            if (sockfd == listenfd)
            {
                // 建立连接
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
                // 连接失败
                if (connfd < 0)
                {
                    printf("errno is: %d\n", errno);
                    continue;
                }
                // 达到最大连接，先不进行连接
                if (http_conn::m_user_count >= MAX_FD)
                {
                    close(connfd);
                    continue;
                }

                // 初始化连接
                users[connfd].init(connfd, client_address);
            }
            // 挂起事件，关闭连接
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                users[sockfd].close_conn();
            }
            // 输入事件，读取
            else if (events[i].events & EPOLLIN)
            {
            }
            // 输出事件，写入
            else if (events[i].events & EPOLLOUT)
            {
            }
        }
    }

    // 释放资源
    close(epollfd);
    close(listenfd);
    delete[] users;
    delete pool;
    return 0;
}