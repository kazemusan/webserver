/*
    #include <unistd.h>
    ssize_t read(int fd, void *buf, size_t count);
        参数：
            fd：文件描述符，open函数返回的
            buf：缓冲区
            count：要读取的字节数

        返回值：
            成功：返回实际读取到的字节数
            失败：返回-1，errno被设置

    ssize_t write(int fd, const void *buf, size_t count);
        参数：
            fd：文件描述符，open函数返回的
            buf：缓冲区
            count：要写入的字节数

        返回值：
            成功：返回实际写入的字节数
            失败：返回-1，errno被设置
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // 1. 打开文件
    int fd = open("a.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // 2. 创建拷贝文件
    int fd2 = open("copy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);

    // 3. 读取源文件，写入拷贝文件
    char buf[1024] = {0};
    int len = 0;
    while ((len = read(fd, buf, sizeof(buf))) > 0) {
        write(fd2, buf, len);
    }

    // 4. 关闭文件
    close(fd);
    close(fd2);
}