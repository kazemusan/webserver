#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main() {

    // 1.判断有名管道文件是否存在
    int ret = access("fifo1", F_OK);
    if(ret == -1) {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo1", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("fifo2", F_OK);
    if(ret == -1) {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo2", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    // 2.以只写的方式打开管道fifo1
    int fdw = open("fifo1", O_WRONLY);
    if(fdw == -1) {
        perror("open");
        exit(0);
    }
    printf("打开管道fifo1成功，等待写入...\n");
    // 3.以只读的方式打开管道fifo2
    int fdr = open("fifo2", O_RDONLY);
    if(fdr == -1) {
        perror("open");
        exit(0);
    }
    printf("打开管道fifo2成功，等待读取...\n");

    char buf[128];

    // 4.循环的写读数据
    while(1) {
        memset(buf, 0, 128);
        // 获取标准输入的数据
        fgets(buf, 128, stdin);
        // 写数据
        ret = write(fdw, buf, strlen(buf));
        if(ret == -1) {
            perror("write");
            exit(0);
        }

        // 5.读管道数据
        memset(buf, 0, 128);
        ret = read(fdr, buf, 128);
        if(ret <= 0) {
            perror("read");
            break;
        }
        printf("buf: %s\n", buf);
    }

    // 6.关闭文件描述符
    close(fdr);
    close(fdw);

    return 0;
}

/*
    memset函数：
        头文件：#include <string.h>
        函数原型：void *memset(void *s, int ch, size_t n);
        函数功能：将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 。
        参数说明：
            s：为指针或是数组，用来指定欲填充内存区域的起始地址。
            ch：为欲填充的字符。ch 的类型为 int，但必须是个能表示 unsigned char 的值，如EOF （即 -1）。
            n：为填充字节的个数。size_t 是一种数据类型，通常无符号整型。
        返回值：返回指向 s 的指针。
        说明：memset() 会将参数 s 所指的内存区域的前 n 个字节的值都设置为参数 c 的ASCII值，然后返回指向 s 的指针。

    fgets函数：
        头文件：#include <stdio.h>
        函数原型：char *fgets(char *s, int size, FILE *stream);
        函数功能：从指定的流 stream 读取一行，并把它存储在 str 所指向的字符串内。当读取 (size-1) 个字符时，或者读取到换行符时，或者到达文件末尾时，它会停止，具体视情况而定。
        参数说明：
            s：指向用来存储所得数据的数组的指针。
            size：存储数据的数组的大小。
            stream：指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
        返回值：如果成功，该函数返回字符串的指针，如果到达文件末尾或者没有读取到任何字符，返回 NULL。
        说明：fgets() 会读取 size-1 个字符，或者遇到换行符为止。如果换行符被读取，则它会被包含在字符串中。如果到达文件末尾时，或者没有读取到任何字符时，它会停止读取，并在末尾自动追加一个空字符（'\0'）。

*/