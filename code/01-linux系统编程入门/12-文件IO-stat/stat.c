/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    int stat(const char *pathname, struct stat *statbuf);
        作用：获取文件信息
        参数：
            -pathname: 要获取信息的文件路径
            -statbuf: 用于保存文件信息的结构体变量

        返回值：
            -成功：返回0
            -失败：返回-1，errno被设置

    int lstat(const char *pathname, struct stat *statbuf);
        作用：获取文件信息，与stat()的区别是：当pathname为符号链接时，获取的是符号链接本身的信息，而不是符号链接指向的文件的信息
        参数：
            -pathname: 要获取信息的文件路径
            -statbuf: 用于保存文件信息的结构体变量

        返回值：
            -成功：返回0
            -失败：返回-1，errno被设置

*/

/*
struct stat {
    dev_t st_dev; // 文件所在设备的ID
    ino_t st_ino; // 文件的inode节点号
    mode_t st_mode; // 文件的类型和权限
    nlink_t st_nlink; // 文件的硬链接数
    uid_t st_uid; // 文件所有者的ID
    gid_t st_gid; // 文件所有者所属组的ID
    dev_t st_rdev; // 若文件为设备文件，则为其设备号
    off_t st_size; // 文件大小，单位为字节
    blksize_t st_blksize; // 文件系统的I/O缓冲区大小
    blkcnt_t st_blocks; // 分配给文件的512字节块数
    time_t st_atime; // 文件最后一次访问时间
    time_t st_mtime; // 文件最后一次修改时间
    time_t st_ctime; // 文件最后一次状态改变时间
};
*/

/*
// 文件类型
#define S_IFMT 0170000 // 文件类型掩码
#define S_IFSOCK 0140000 // 套接字文件
#define S_IFLNK 0120000 // 符号链接文件
#define S_IFREG 0100000 // 普通文件
#define S_IFBLK 0060000 // 块设备文件
#define S_IFDIR 0040000 // 目录文件
#define S_IFCHR 0020000 // 字符设备文件
#define S_IFIFO 0010000 // 管道文件
// 文件权限
#define S_IRWXU 00700 // 文件所有者的读、写、执行权限
#define S_IRUSR 00400 // 文件所有者的读权限
#define S_IWUSR 00200 // 文件所有者的写权限
#define S_IXUSR 00100 // 文件所有者的执行权限
#define S_IRWXG 00070 // 文件所属组的读、写、执行权限
#define S_IRGRP 00040 // 文件所属组的读权限
#define S_IWGRP 00020 // 文件所属组的写权限
#define S_IXGRP 00010 // 文件所属组的执行权限
#define S_IRWXO 00007 // 其他用户的读、写、执行权限
#define S_IROTH 00004 // 其他用户的读权限
#define S_IWOTH 00002 // 其他用户的写权限
#define S_IXOTH 00001 // 其他用户的执行权限
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // 1. 获取文件信息
    struct stat statbuf;
    int ret = stat("a.txt", &statbuf);
    if (ret == -1) {
        perror("stat");
        return -1;
    }

    // 2. 打印文件信息
    printf("文件大小：%ld\n", statbuf.st_size);

    return 0;
}