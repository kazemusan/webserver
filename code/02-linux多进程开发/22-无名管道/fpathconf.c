/*
    1. 无名管道的大小是有限制的
    2. 无名管道的大小是由系统决定的
    3. 无名管道的大小是可以修改的
    4. 无名管道的大小是可以查看的

    查看无名管道的大小
    命令：man fpathconf 或 ulimit -a
        相关宏：_PC_PIPE_BUF // 代表管道的大小
        
    函数：long fpathconf(int fd, int name);
        功能：获取文件的配置信息
        参数：
            int fd 文件描述符
            int name 配置信息的名称
            name 可选宏：
                _PC_PIPE_BUF 代表管道的大小
                _PC_NAME_MAX 代表文件名的最大长度
                _PC_PATH_MAX 代表路径的最大长度
        返回值：
            成功 返回配置信息
            失败 -1

*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    int pipefd[2];

    int ret = pipe(pipefd);

    // 获取管道的大小
    long size = fpathconf(pipefd[0], _PC_PIPE_BUF);

    printf("pipe size : %ld\n", size);

    return 0;
}