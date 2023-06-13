/*

    #include <unistd.h>
    #include <fcntl.h>

    int fcntl(int fd, int cmd, ...);
    参数：
        fd : 表示需要操作的文件描述符
        cmd: 表示对文件描述符进行如何操作
            - F_DUPFD : 复制文件描述符,复制的是第一个参数fd，得到一个新的文件描述符（返回值）
                int ret = fcntl(fd, F_DUPFD);

            - F_GETFL : 获取指定的文件描述符文件状态flag
              获取的flag和我们通过open函数传递的flag是一个东西。

            - F_SETFL : 设置文件描述符文件状态flag
              必选项：O_RDONLY, O_WRONLY, O_RDWR 不可以被修改
              可选性：O_APPEND, O_NONBLOCK
                O_APPEND 表示追加数据
                NONBLOK 设置成非阻塞
        
        阻塞和非阻塞：描述的是函数调用的行为。

        返回值：
            - 成功：返回对应的操作结果，如F_DUPFD返回的是新的文件描述符
            - 失败：-1

    文件状态：
        文件状态是由文件描述符的文件状态flag决定的。
        文件状态flag是通过open函数的第二个参数来设置的。
        文件状态flag是一个int类型的值，这个值是由多个选项通过或运算得到的。
        文件状态flag的选项：
            - 必选项：O_RDONLY, O_WRONLY, O_RDWR
            - 可选项：O_APPEND, O_NONBLOCK
        文件状态flag的获取和设置：
            - 获取：fcntl(fd, F_GETFL);
            - 设置：fcntl(fd, F_SETFL, flag);
        文件状态flag的修改：
            - 必选项：不可以被修改
            - 可选项：可以被修改
        文件状态flag的作用：
            - 必选项：必须要指定，否则open函数会报错
            - 可选项：可以不指定，如果不指定，那么就使用默认值
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {

    // 1.复制文件描述符
    // int fd = open("1.txt", O_RDONLY);
    // int ret = fcntl(fd, F_DUPFD);

    // 2.修改或者获取文件状态flag
    int fd = open("1.txt", O_RDWR);
    if(fd == -1) {
        perror("open");
        return -1;
    }

    // 获取文件描述符状态flag
    int flag = fcntl(fd, F_GETFL);
    if(flag == -1) {
        perror("fcntl");
        return -1;
    }
    flag |= O_APPEND;   // flag = flag | O_APPEND

    // 修改文件描述符状态的flag，给flag加入O_APPEND这个标记
    int ret = fcntl(fd, F_SETFL, flag);
    if(ret == -1) {
        perror("fcntl");
        return -1;
    }

    char * str = "nihao";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}