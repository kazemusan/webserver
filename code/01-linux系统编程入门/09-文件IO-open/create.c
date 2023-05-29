/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    int open(const char *pathname, int flags， mode_t mode);
        参数：
            -pathname: 要打开的文件路径

            -flags: 打开文件的方式
                必选：
                    -O_RDONLY: 只读方式打开
                    -O_WRONLY: 只写方式打开
                    -O_RDWR: 读写方式打开
                可选：
                    -O_CREAT: 文件不存在则创建
                    -O_EXCL: 与O_CREAT同用，若文件存在则报错
                    -O_TRUNC: 文件存在且为只写或读写方式打开时，文件长度截断为0
                    -O_APPEND: 文件存在且为只写或读写方式打开时，写入数据追加到文件尾部
                    -O_NONBLOCK: 以非阻塞方式打开文件

                flags = 必选 | 可选 | 可选 | ...
                flags 是一个整型数，每个选项都是一个位标志，可以用位或运算符连接在一起

            -mode: 文件权限，只有在创建文件时才有效
                -S_IRUSR: 文件所有者具有读权限
                -S_IWUSR: 文件所有者具有写权限
                -S_IXUSR: 文件所有者具有执行权限
                -S_IRGRP: 文件所属组具有读权限
                -S_IWGRP: 文件所属组具有写权限
                -S_IXGRP: 文件所属组具有执行权限
                -S_IROTH: 其他用户具有读权限
                -S_IWOTH: 其他用户具有写权限
                -S_IXOTH: 其他用户具有执行权限

                也可设置为八进制数，如： 0777
                最终权限 = mode & ~umask // umask为文件模式创建屏蔽字
                umask 022: 文件所有者具有读写执行权限，所属组、其他用户具有读写权限
                umask 002: 文件所有者、所属组具有读写执行权限，其他用户具有读写执行权限

        返回值：
            -成功：返回文件描述符
            -失败：返回-1

    errorno：linux系统函数库的全局变量，用于保存最近错误编号
    perror(const char *s): stdio.h库函数，用于打印错误信息
        s: 用于指定错误信息前的提示信息
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    // 1.打开文件
    int fd = open("a.txt", O_RDWR | O_CREAT | O_EXCL, 0777);
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