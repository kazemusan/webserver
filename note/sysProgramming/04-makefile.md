## table of contents
- [简介](#简介)
- [工作原理](#工作原理)
- [变量](#变量)
  - [自定义变量](#自定义变量)
  - [系统变量](#系统变量)
  - [获取变量的值](#获取变量的值)
- [函数](#函数)
    - [自定义函数](#自定义函数)
    - [系统函数](#系统函数)
- [语法](#语法)
    - [条件判断](#条件判断)
    - [循环](#循环)
    - [模式匹配](#模式匹配)

## [简介](#table-of-contents)
> makefile 是一个用来编译程序的工具，它可以根据程序的依赖关系，自动编译程序。  
> makefile 由一系列规则组成，每条规则的格式如下：  
> ```makefile
> target: prerequisites
>     command
> ```
>> target 是规则的目标，prerequisites 是规则的依赖，command 是规则的命令。  
>> target 是一个文件，prerequisites 是 target 的依赖，command 是编译 target 的命令。

## [工作原理](#table-of-contents)
> makefile 会根据规则的依赖关系，自动编译程序。  
>> 命令执行之前，会先检查依赖是否存在，如果依赖不存在，就会执行依赖的规则。  
>> 命令执行前还会检查更新时间，如果依赖的更新时间比目标的更新时间要新，就会执行依赖的规则。  

## [变量](#table-of-contents)
### [自定义变量](#table-of-contents)
> 变量的格式如下：
> ```makefile
> name = value
> ```
>> name 是变量的名称，value 是变量的值。

### [系统变量](#table-of-contents)
> makefile 中有一些系统变量，可以直接使用。  
>> ```text
>> $@ 表示目标
>> $^ 表示所有依赖
>> $< 表示第一个依赖
>> AR 表示静态库的打包命令
>> CC 表示C编译命令
>> CXX 表示C++编译命令
>> CFLAGS 表示编译选项
>> LDFLAGS 表示链接选项
>> ```

### [获取变量的值](#table-of-contents)
> 可以使用 $(name) 的方式获取变量的值。

## [函数](#table-of-contents)
### [自定义函数](#table-of-contents)
> 函数的格式如下：
> ```makefile
> name = $(function arguments)
> ```
>> name 是函数的名称，function 是函数的名称，arguments 是函数的参数。

### [系统函数](#table-of-contents)
> makefile 中有一些系统函数，可以直接使用。
>> ```makefile
>> $(wildcard pattern) # 表示获取匹配 pattern 的文件列表
>> $(patsubst pattern, replacement, text) # 表示将 text 中匹配 pattern 的字符串替换为 replacement
>> $(notdir names...) # 表示获取 names 中的文件名
>> $(dir names...) # 表示获取 names 中的目录名
>> $(subst from, to, text) # 表示将 text 中的 from 替换为 to
>> $(addsuffix suffix, names...) # 表示给 names 中的每个字符串添加后缀 suffix
>> $(addprefix prefix, names...) # 表示给 names 中的每个字符串添加前缀 prefix
>> $(join list1, list2) # 表示将 list1 和 list2 中的字符串拼接起来
>> $(sort list) # 表示对 list 中的字符串进行排序
>> $(word n, text) # 表示获取 text 中的第 n 个字符串
>> $(words text) # 表示获取 text 中的字符串个数
>> $(firstword names...) # 表示获取 names 中的第一个字符串
>> $(filter pattern..., text) # 表示获取 text 中匹配 pattern 的字符串
>> $(filter-out pattern..., text) # 表示获取 text 中不匹配 pattern 的字符串
>> $(shell command) # 表示执行 command 命令
>> ```

## [语法](#table-of-contents)
### [条件判断](#table-of-contents)
> 条件判断的格式如下：
> ```makefile
> ifeq (arg1, arg2)
>     command
> else
>     command
> endif
> ```
>> arg1 和 arg2 是两个参数，command 是命令。

### [循环](#table-of-contents)
> 循环的格式如下：
> ```makefile
> for name in list
>     command
> endfor
> ```
>> name 是变量的名称，list 是变量的值，command 是命令。

### [模式匹配](#table-of-contents)
> 模式匹配的格式如下：
> ```makefile
> %.o: %.c
>     $(CC) $(CFLAGS) -c $< -o $@
> ```
>> %.o 表示所有的 .o 文件，%.c 表示所有的 .c 文件，$< 表示第一个依赖，$@ 表示目标。
