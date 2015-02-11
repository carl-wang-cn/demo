##pipe

###pipe匿名管道

匿名管道由于没有名字, 因此只能由有亲缘关系的进程使用

```cpp
#include <unistd.h>
int pipe(int fd[2]);
返回: 成功返回0, 出错返回-1

该函数返回两个文件描述符: fd[0]和fd[1], 前者打开来读, 后者打开来写.

```

另一个关于管道的例子, 标准I/O函数库提供了popen函数, 它创建一个管道并启动另一个
进程, 该进程要么从该管道读出标准输入, 要么往该管道写入标准输出

```cpp
#include <stdio.h>

FILE *popen(const char *command, const char *type);
返回: 成功返回文件指针，失败返回NULL
注意: popen不对标准错误输出做任何处理, 只有标准输出才被重定向到由它创建的管道

int pclose(FILE *stream);
返回: 成功则为shell的终止状态, 失败为-1

command是一个shell命令行, popen在调用进程和所指定的命令之间创建一个管道

- 如果type为r, 调用进程读入command的标准输出
- 如果type为w, 调用进程写到command的标准输入

```

* pipe01.c演示了父子进程间通过pipe实现半双工通信
* pipe02.c演示了使用popen创建管道

###FIFO 有名管道

每个FIFO有一个路径名与之关联, 从而允许无亲缘关系的进程访问同一个FIFO。

```cpp
#include <sys/types.h>
#include <sys/stat.h>

inf mkfifo(const char *pathname, mode_t mode);
返回: 成功返回0, 失败返回-1

mkfifo函数已经隐含指定了O_CREAT | O_EXCL, 也就是说, 它要么创建一个新的FIFO,
要么返回一个EEXIST错误(如果所指定名字的FIFO已经存在)。此处可参考示例中的正
确创建FIFO的步骤

- 创建出一个FIFO后, 它必须或者打开来读, 或者打开来写。不能打开来既读又写,
  因为它是**半双工**的
- 如果一个FIFO, 当前尚没有进程打开它来写, 那么此时打开该FIFO来读的进程将会阻塞
- 对管道或FIFO的write总是往末尾添加数据, 对它们的read总是从开头返回数据。
- 如果对管道或FIFO执行lseek, 会返回ESPIPE错误
- 当对一个管道或FIFO的最终close发生时, 该管道或FIFO中的任何残留数据都被丢弃

```

* fifo_read.c 中从文件data.txt中读取内容, 写入到FIFO中
* fifo_write.c 中从FIFO中读取内容, 写入到datafromfifo.txt中
* data.txt 是测试文件
* datafromfifo.txt 是fifo_write.c运行后生成的文件

###管道和FIFO的区别

* 创建并打开一个管道, 只需要调用pipe；
  创建并打开一个FIFO则需在调用mkdifo后再调用open
* 管道在所有进程最终都关闭它之后自动消失。
  FIFO的名字则只有通过调用unlink才从文件系统中删除
* FIFO需要额外调用的好处: FIFO在文件系统中有一个名字, 该名字允许某个进程创建一个
  FIFO, 与它**无亲缘关系**的  另一个进程来打开这个FIFO, 这对管道来说是不可能的(
  不考虑传递描述符的情况)

###管道和FIFO的限制

系统加于管道和FIFO的唯一限制为:

* OPEN_MAX 一个进程在任意时刻打开的最大描述符数
* PIPE_BUF 可原子地写往一个管道或FIFO的最大数据量

* get_sys_conf.c 获取当前系统的OPEN_MAX和PIPE_BUF值


###将一个描述符设为非阻塞两种方式

1. 调用open的时候可指定O_NONBLOCK标志
2. 如果一个描述符已打开, 那么可以调用fcntl以启用O_NONBLOCK标志。
   对于管道来说, 必须使用这种技术, 因为管道没有open调用, 在pipe中也无法指定O_NONBLOCK标志

**FIFO的真正优势**表现在服务器可以是一个长期运行的进程（例如守护进程）, 而且与其客户可以无亲缘关系。
作为服务器的守护进程以某个众所周知的路径名创建一个FIFO, 并打开该FIFO来读。此后某个
时刻启动的客户打开该FIFO来写, 并将其命令或给守护进程的其他任何东西通过该FIFO发送出去。
使用FIFO很容易实现这种形式的单向通信(client->server)。如果守护进程需要向客户发送
回一些东西, 那就需要点技巧(例如客户需要通知守护进程要打开哪个FIFO可以给其发消息)

