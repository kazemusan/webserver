/*
    // 打开一个目录
    #include <sys/types.h>
    #include <dirent.h>
    DIR *opendir(const char *name);
        参数：
            - name: 需要打开的目录的名称
        返回值：
            DIR * 类型，理解为目录流
            错误返回NULL


    // 读取目录中的数据
    #include <dirent.h>
    struct dirent *readdir(DIR *dirp);
        - 参数：dirp是opendir返回的结果
        - 返回值：
            struct dirent，代表读取到的文件的信息
            读取到了末尾或者失败了，返回NULL

    // 关闭目录
    #include <sys/types.h>
    #include <dirent.h>
    int closedir(DIR *dirp);

*/

/*
// DIR结构体
struct _dirdesc {
    int     dd_fd;      // 目录对应的文件描述符
    long    dd_loc;     // 目录文件中的偏移
    long    dd_size;    // 目录文件的大小
    char    *dd_buf;    // 目录文件的缓冲区
    int     dd_len;     // 目录文件缓冲区的大小
    long    dd_seek;    // 目录文件中下一个要读取的位置
    long    dd_rewind;  // 目录文件中下一个要读取的位置
    struct dirent   *dd_bufptr; // 目录文件中下一个要读取的位置
    struct dirent   dd_dent;    // 目录文件中下一个要读取的位置
};

// dirent结构体
struct dirent {
    ino_t          d_ino;       // inode number 索引节点号
    off_t          d_off;       // offset to the next dirent 在目录文件中的偏移
    unsigned short d_reclen;    // length of this record 文件名长
    unsigned char  d_type;      // type of file 文件类型
    char           d_name[256]; // filename 文件名，最长256字节
};

// d_type的类型
#define DT_UNKNOWN       0  // 未知的类型
#define DT_FIFO          1  // 管道文件
#define DT_CHR           2  // 字符设备文件
#define DT_DIR           4  // 目录文件
#define DT_BLK           6  // 块设备文件
#define DT_REG           8  // 普通文件
#define DT_LNK          10  // 符号链接文件
#define DT_SOCK         12  // 套接字文件
#define DT_WHT          14  // 白色，不常用
*/
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getFileNum(const char * path);

// 读取某个目录下所有的普通文件的个数
int main(int argc, char * argv[]) {

    if(argc < 2) {
        printf("%s path\n", argv[0]);
        return -1;
    }

    int num = getFileNum(argv[1]);

    printf("普通文件的个数为：%d\n", num);

    return 0;
}

// 用于获取目录下所有普通文件的个数
int getFileNum(const char * path) {

    // 1.打开目录
    DIR * dir = opendir(path);

    if(dir == NULL) {
        perror("opendir");
        exit(0);
    }

    struct dirent *ptr;

    // 记录普通文件的个数
    int total = 0;

    while((ptr = readdir(dir)) != NULL) {

        // 获取名称
        char * dname = ptr->d_name;

        // 忽略掉. 和..
        if(strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0) {
            continue;
        }

        // 判断是否是普通文件还是目录
        if(ptr->d_type == DT_DIR) {
            // 目录,需要继续读取这个目录
            char newpath[256];
            sprintf(newpath, "%s/%s", path, dname);
            total += getFileNum(newpath);
        }

        if(ptr->d_type == DT_REG) {
            // 普通文件
            total++;
        }


    }

    // 关闭目录
    closedir(dir);

    return total;
}