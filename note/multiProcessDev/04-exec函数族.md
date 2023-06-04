## table of contents
- [exec函数族](#exec函数族)
- [exec函数族的函数](#exec函数族的函数)
- [exec函数族的函数的参数](#exec函数族的函数的参数)
- [exec函数族的函数的返回值](#exec函数族的函数的返回值)
- [exec函数族的函数的区别](#exec函数族的函数的区别)
- [exec函数族的函数的使用](#exec函数族的函数的使用)
  - [execl](#execl)
  - [execlp](#execlp)
  - [execle](#execle)
  - [execv](#execv)
  - [execvp](#execvp)
  - [execvpe](#execvpe)

## [exec函数族](#table-of-contents)
```text
exec函数族：用于执行一个新的程序，新的程序会替换当前进程的代码段、数据段、堆段和栈段。

exec 函数族的函数执行成功后，不会返回，如果返回，说明执行失败了。因为调用进程的实体，包括代码段、数据段、堆段和栈段都被新的程序替换了，调用进程的实体已经不存在了，所以，调用进程也就无法返回了。
```

### [exec函数族的函数](#table-of-contents)
```text
exec函数族的函数有6个，分别是：
int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char * const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

### [exec函数族的函数的参数](#table-of-contents)
```text
1. path: 要执行的程序的路径
2. file: 要执行的程序的文件名
3. arg: 要执行的程序的命令行参数
4. envp: 要执行的程序的环境变量
5. argv: 要执行的程序的命令行参数
```

### [exec函数族的函数的返回值](#table-of-contents)
```text
成功：不会返回
失败：-1
```

### [exec函数族的函数的区别](#table-of-contents)
```text
1. execl函数族的函数，参数列表是可变参数，参数列表的最后一个参数必须是NULL，参数列表的第一个参数是要执行的程序的路径或者文件名。
2. execv函数族的函数，参数列表是数组，数组的最后一个元素必须是NULL，数组的第一个元素是要执行的程序的路径或者文件名。
3. execlp函数族的函数，参数列表是可变参数，参数列表的最后一个参数必须是NULL，参数列表的第一个参数是要执行的程序的文件名，文件名不需要路径，系统会自动到环境变量PATH中查找。
4. execvp函数族的函数，参数列表是数组，数组的最后一个元素必须是NULL，数组的第一个元素是要执行的程序的文件名，文件名不需要路径，系统会自动到环境变量PATH中查找。
5. execle函数族的函数，参数列表是可变参数，参数列表的最后一个参数必须是NULL，参数列表的第一个参数是要执行的程序的路径或者文件名，参数列表的倒数第二个参数是要执行的程序的环境变量。
6. execvpe函数族的函数，参数列表是数组，数组的最后一个元素必须是NULL，数组的第一个元素是要执行的程序的文件名，参数列表的倒数第二个参数是要执行的程序的环境变量。
```

### [exec函数族的函数的使用](#table-of-contents)
#### [execl](#table-of-contents)
```c
/*
    #include <unistd.h>

    int execl(const char *path, const char *arg, ...);
        函数的作用：用于执行一个新的程序，新的程序会替换当前进程的代码段、数据段、堆段和栈段。
        参数：
            path: 要执行的程序的路径
            arg: 要执行的程序的命令行参数
        返回值：
            成功：不会返回
            失败：-1
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("main函数开始执行...\n");
    execl("/bin/ls", "ls", "-l", NULL);
    printf("main函数结束执行...\n");
    return 0;
}
```

#### [execlp](#table-of-contents)
```c
/*
    #include <unistd.h>

    int execlp(const char *file, const char *arg, ...);
        函数的作用：用于执行一个新的程序，新的程序会替换当前进程的代码段、数据段、堆段和栈段。
        参数：
            file: 要执行的程序的文件名
            arg: 要执行的程序的命令行参数
        返回值：
            成功：不会返回
            失败：-1
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("main函数开始执行...\n");
    execlp("ls", "ls", "-l", NULL);
    printf("main函数结束执行...\n");
    return 0;
}
```

#### [execle](#table-of-contents)
```c
/*
    #include <unistd.h>

    int execle(const char *path, const char *arg, ..., char * const envp[]);
        函数的作用：用于执行一个新的程序，新的程序会替换当前进程的代码段、数据段、堆段和栈段。
        参数：
            path: 要执行的程序的路径
            arg: 要执行的程序的命令行参数
            envp: 要执行的程序的环境变量
        返回值：
            成功：不会返回
            失败：-1
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("main函数开始执行...\n");
    char *envp[] = {"PATH=/bin", NULL};
    execle("/bin/ls", "ls", "-l", NULL, envp);
    printf("main函数结束执行...\n");
    return 0;
}
```

#### [execv](#table-of-contents)
```c
/*
    #include <unistd.h>

    int execv(const char *path, char *const argv[]);
        函数的作用：用于执行一个新的程序，新的程序会替换当前进程的代码段、数据段、堆段和栈段。
        参数：
            path: 要执行的程序的路径
            argv: 要执行的程序的命令行参数
        返回值：
            成功：不会返回
            失败：-1
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("main函数开始执行...\n");
    char *argv[] = {"ls", "-l", NULL};
    execv("/bin/ls", argv);
    printf("main函数结束执行...\n");
    return 0;
}
```

#### [execvp](#table-of-contents)
```c
/*
    #include <unistd.h>

    int execvp(const char *file, char *const argv[]);
        函数的作用：用于执行一个新的程序，新的程序会替换当前进程的代码段、数据段、堆段和栈段。
        参数：
            file: 要执行的程序的文件名
            argv: 要执行的程序的命令行参数
        返回值：
            成功：不会返回
            失败：-1
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("main函数开始执行...\n");
    char *argv[] = {"ls", "-l", NULL};
    execvp("ls", argv);
    printf("main函数结束执行...\n");
    return 0;
}
```

#### [execvpe](#table-of-contents)
```c
/*
    #include <unistd.h>

    int execvpe(const char *file, char *const argv[], char *const envp[]);
        函数的作用：用于执行一个新的程序，新的程序会替换当前进程的代码段、数据段、堆段和栈段。
        参数：
            file: 要执行的程序的文件名
            argv: 要执行的程序的命令行参数
            envp: 要执行的程序的环境变量
        返回值：
            成功：不会返回
            失败：-1
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("main函数开始执行...\n");
    char *argv[] = {"ls", "-l", NULL};
    char *envp[] = {"PATH=/bin", NULL};
    execvpe("ls", argv, envp);
    printf("main函数结束执行...\n");
    return 0;
}
```