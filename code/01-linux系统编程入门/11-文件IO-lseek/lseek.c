/*
    // C库函数 - fseek()
    #include <stdio.h>
    int fseek(FILE *stream, long offset, int whence);
        参数：
            stream：文件指针
            offset：偏移量
            whence：偏移起始位置
                SEEK_SET：文件起始位置
                SEEK_CUR：当前位置
                SEEK_END：文件末尾位置

        返回值：
            成功：返回0
            失败：返回-1，errno被设置

    // linux系统函数 - lseek()
    #include <sys/types.h>
    #include <unistd.h>
    off_t lseek(int fd, off_t offset, int whence);
        参数：
            fd：文件描述符，open函数返回的
            offset：偏移量
            whence：偏移起始位置
                SEEK_SET：文件起始位置
                SEEK_CUR：当前位置
                SEEK_END：文件末尾位置

        返回值：
            成功：返回新的偏移量
            失败：返回-1，errno被设置

        作用：
            1. 移动文件指针到指定位置
            lseek(fd, 0, SEEK_SET); // 移动到文件起始位置

            2. 获取当前文件指针位置
            lseek(fd, 0, SEEK_CUR); // 获取当前位置

            3. 获取文件大小
            lseek(fd, 0, SEEK_END); // 移动到文件末尾位置
            off_t size = lseek(fd, 0, SEEK_CUR); // 获取当前位置，即文件大小

            4. 文件拓展
            lseek(fd, 100, SEEK_END); // 文件拓展100字节

            5. 文件截断
            lseek(fd, 0, SEEK_END); // 移动到文件末尾位置
            ftruncate(fd, lseek(fd, 0, SEEK_CUR)); // 截断文件到当前位置

*/
// 文件拓展
// 下载文件时，先创建一个空文件，然后再写入数据
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // 1. 打开文件
    int fd = open("a.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // 2. 文件拓展
    lseek(fd, 100, SEEK_END);

    // 3. 写入数据
    write(fd, "hello", 5);

    // 4. 关闭文件
    close(fd);

    return 0;
}
