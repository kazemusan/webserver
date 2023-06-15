#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {    

    // 1.创建一个共享内存
    int shmid = shmget(100, 4096, IPC_CREAT|0664);
    printf("shmid : %d\n", shmid);
    
    // 2.和当前进程进行关联
    void * ptr = shmat(shmid, NULL, 0);

    char * str = "helloworld";

    // 3.写数据
    memcpy(ptr, str, strlen(str) + 1);

    printf("按任意键继续\n");
    getchar();

    // 4.解除关联
    shmdt(ptr);

    // 5.删除共享内存
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

/*
    memcpy(ptr, str, strlen(str) + 1);
    功能：
        将str指向的字符串拷贝到ptr指向的共享内存中
    参数：
        ptr：共享内存的首地址
        str：要拷贝的字符串
        strlen(str) + 1：要拷贝的字符串的长度
    返回值：
        拷贝的字符串的首地址
*/