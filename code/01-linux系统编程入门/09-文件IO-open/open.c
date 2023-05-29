/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    int open(const char *pathname, int flags);
        参数：
            -pathname: 要打开的文件路径
            -flags: 打开文件的方式
                -O_RDONLY: 只读方式打开
                -O_WRONLY: 只写方式打开
                -O_RDWR: 读写方式打开
            返回值：
                -成功：返回文件描述符
                -失败：返回-1

    errorno：linux系统函数库的全局变量，用于保存最近错误编号
    perror(const char *s): stdio.h库函数，用于打印错误信息
        s: 用于指定错误信息前的提示信息

    int open(const char *pathname, int flags, mode_t mode);
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    // 1.打开文件
    int fd = open("a.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    printf("fd = %d\n", fd);

    // 2.关闭文件
    close(fd);

    return 0;
}