/*
    #include <sys/epoll.h>

    int epoll_create(int size);
    作用：创建一个epoll实例
    参数：
        size：epoll实例的大小，这个参数在epoll的实现中并没有什么作用，只是给内核一个提示，告诉内核这个epoll实例中需要存放多少个文件描述符
    返回值：
        成功：epoll实例的文件描述符
        失败：-1

    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    作用：将文件描述符相关的检测信息添加到epoll实例中
    参数：
        epfd：epoll实例的文件描述符
        op：操作类型
            EPOLL_CTL_ADD：添加
            EPOLL_CTL_MOD：修改
            EPOLL_CTL_DEL：删除
        fd：需要添加到epoll实例中的文件描述符
        event：需要添加到epoll实例中的文件描述符相关的检测信息
    返回值：
        成功：0
        失败：-1

    int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
    作用：等待文件描述符的状态发生变化
    参数：
        epfd：epoll实例的文件描述符
        events：用来保存检测到的文件描述符相关的检测信息
        maxevents：events数组的大小
        timeout：超时时间，-1表示阻塞等待，0表示非阻塞，其他值表示等待的最大时间
    返回值：
        -1：失败
        0：超时
        >0：就绪的文件描述符的个数
*/
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

int main() {

    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定
    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    // 监听
    listen(lfd, 8);

    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(100);

    // 将监听的文件描述符相关的检测信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];

    while(1) {

        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }

        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++) {

            int curfd = epevs[i].data.fd;

            if(curfd == lfd) {
                // 监听的文件描述符有数据达到，有客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                if(epevs[i].events & EPOLLOUT) {
                    continue;
                }   
                // 有数据到达，需要通信
                char buf[1024] = {0};
                int len = read(curfd, buf, sizeof(buf));
                if(len == -1) {
                    perror("read");
                    exit(-1);
                } else if(len == 0) {
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                } else if(len > 0) {
                    printf("read buf = %s\n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }

            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}