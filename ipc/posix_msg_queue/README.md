Posix消息队列
===

###基本特性
---

1. 在某个进程往一个队列写入消息之前, 并不需要另外某个进程在该队列上等待消息的到达.
   这个与管道和FIFO是相反的, 对后两者来说, 除非读出者已存在, 否则先有写入者是没有意义的.
2. 消息队列具有随内核的持续性.一个进程可以往某个队列写入一些消息, 然后终止, 再让
   另外一个进程在以后某个时刻读出这些消息
3. 队列属性

**相关API**

```cpp
#include <mqueue.h>

mqd_t mq_open(const char *name, int oflag, /* mode_t mode, struct mq_attr *attr */);
返回：成功返回消息队列描述符(作为其余7个消息队列函数的第一个参数), 失败返回-1

int mq_close(mqd_t mqdes);
返回：成功返回0, 失败返回-1
注意: 调用进程可以不再使用该描述符, 但其消息队列并不从系统中删除.
      要从系统中删除用作mq_open第一个参数的某个name, 必须调用mq_unlink

int mq_unlink(const char *name);
返回：成功返回0, 失败返回-1

每个消息队列有一个保存其当前打开着描述符数的引用计数器, 当一个消息队列的
引用计数仍大于0时, 其name就能删除, 但是该队列的析构要到最后一个mq_close
发生时才进行队列及其上的消息, 一直存在到调用mq_unlink并让它的引用计数达
到0以删除该队列为止

struct mq_attr
{
    long mq_flags;    // message queue flags: 0, O_NONBLOCK
    long mq_maxmsg;   // max number of messages allowed on queue
    long mq_msgsize;  // max size of a message (in bytes)
    long mq_curmsgs;  // number of messages currently on queue 只能获取, 不能设置
};

int mq_getattr(mqd_t mqdes, struct mq_attr *attr);
int mq_setattr(mqd_t mqdes, const struct mq_attr *attr, struct mq_attr *oattr);
返回：成功返回0, 失败返回-1

- mq_setattr 只能设置flags属性, 用于设置或清除阻塞标志, 其他3个属性被忽略.
- mq_maxmsg, mq_msgsize 只能在mq_open的时候设置, 这两个属性必须 同时指定.
- mq_getattr 用于获取队列的属性
- mq_receive 总是返回队列中最高优先级的最早消息

int mq_send(mqd_t mqdes, const char *ptr, size_t len, unsigned int prio);
返回：成功返回0, 失败返回-1

ssize_t mq_receive(mqd_t mqdes, char *ptr, size_t len, unsigned int *priop);
返回：成功则返回消息中字节数, 失败返回-1

注意:
- mq_receive第二个参数ptr大小的限制, 要>=attr.msgsize才行, 这意味着使用Posix消息
  队列的大多数应用程序必须在打开某个队列后调用mq_getattr确定最大消息大小, 然后分配
  一个或多个那样大小的读缓冲区
- 每个消息都有一个优先级, 它是一个小于MQ_PRIO_MAX的无符号整数.如果应用不必使用优先级
  不同的消息, 那就给mq_send指定值为0的优先级

int mq_notify(mqd_t mqdes, const struct sigevent *notification);

```

* mq_create.c 创建posix消息队列
* mq_unlink.c 删除posix消息队列
* mq_send.c 向posix消息队列发送消息
* mq_receive.c 从posix消息队列收取消息


###Posix消息队列限制
---

* MQ_OPEN_MAX 一个进程能够同时拥有的打开着消息队列的最大数目(Posix要求它至少为8)
* MQ_PRIO_MAX 任意消息的最大优先级+1(Posix要求它至少为32)
* get_sys_conf.c 获取当前系统的MQ_OPEN_MAX和MQ_PRIO_MAX值


###Posix消息队列的异步事件通知
---

Posix消息队列允许异步事件通知, 以告知何时有一个消息放置到了某个空消息队列中.有两种方式可供选择

1. 产生一个信号
2. 创建一个线程来执行一个指定的函数

```cpp
#include <signal.h>

union sigval
{
    int sival_int;   // integer value
    void *sival_ptr; // pointer value
};

struct sigevent
{
    int sigev_notify;         // SIGEV_{NONE, SIGNAL, THREAD}
    union sigval sigev_value; // passed to signal handler or thread

    // following two if SIGEV_THREAD
    void (*sigev_notify_function)(union sigval);
    pthread_attr_t *sigev_notify_attributes;
}

#include <mqueue.h>

int mq_notify(mqd_t mqdes, const struct sigevent *notification);
返回：成功返回0, 失败返回-1

该函数为指定队列建立或删除异步事件通知

该函数的若干使用规则：

1. 如果notification参数非空, 那么当前进程希望在有一个消息到达所指定的先前为空
   的队列时得到通知.（我们说该进程被注册为接收该队列的通知）
2. 如果notification参数为NULL,  而且当前进程目前被注册为接收所指定队列的通知,
   那么已存在的注册将被撤销
3. 任意时刻只有一个进程可以被注册为接收某个给定队列的通知
4. 当有一个消息到达某个先前为空的队列, 而且已有一个进程被注册为接受该队列的通
   知时, 只有在没有任何线程阻塞在该队列的mq_receive调用中的前提下, 通知才会发
   出.也就是说, 在mq_receive调用中的阻塞比任何通知的注册都优先
5. 当该通知被发送给它的注册进程时, 其注册即被撤销.该进程必须再次调用mq_notify以重新注册.

```

####产生一个信号
---

**异步信号安全函数**

|command list||||
|:----------|:-----------|:-----------|:-----------|
|access     |fpathconf   |rename      |sysconf     |
|aio_return |fstat       |rmdir       |tcdrain     |
|aio_suspend|fsync       |sem_post    |tcflow      |
|alarm      |getegid     |setgid      |tcflush     |
|cfgetispeed|geteuid     |setpgid     |tcgetattr   |
|cfgetospeed|getgid      |setsid      |tcgetpgrp   |
|cfsetispeed|getgroups   |setuid      |tcsendbreak |
|cfsetospeed|getpgrp     |sigaction   |tcsetattr   |
|chdir|getpid|sigaddset|tcsetpgrp|
|chmod|getppid|sigdelset|time|
|chown|getuid|sigemptyset|timer_getoverrun|
|clock_gettime|kill|sigfillset|timer_gettime|
|close|link|sigismember|timer_settime|
|creat|lseek|signal|times|
|dup|mkdir|sigpause|umask|
|dup2|mkfifo|sigpending|uname|
|execle|open|sigprocmask|unlink|
|execve|pathconf|sigqueue|utime|
|_exit|pause|sigset|wait|
|fcntl|pipe|sigsuspend|waitpid|
|fdatasync|raise|sleep|write|
|fork|read|stat||

**注意：** 没有列在上表中的函数, 不可以在信号处理程序中调用.注意所有标准I/O函数和pthread_XXX函数都没有列在其中.

* mq_notify01.c 使用信号通知读Posix消息队列

```cpp
#include <signal.h>

int sigwait(const sigset_t *set, int *sig);
返回: 成功返回0, 失败返回Exxx错误码

sigwait一直阻塞到set中指定的信号中有一个或多个待处理, 这时它返回其中一个信号, 存到
sig中, 返回值为0. 这个过程称为"同步地等待一个异步事件"

```

* mq_notify02.c 伴随sigwait使用mq_notify, 使用sigwait代替信号处理程序的信号通知

消息队列描述符(mqd_t变量)不是"普通"描述符, 它不能用在select或poll中, 然而我们可以
采用"曲线救国"的思想, 伴随一个管道和mq_notify函数使用它们.

* mq_notify03.c 使用select的Posix消息队列

####创建一个线程
---

异步事件通知的另一种方式是把sigev_notify设置成SIGEV_THREAD, 这会创建一个新的线程.该线程
调用由sigev_notify_function指定的函数, 所用的参数由sigev_value指定.新线程的线程属性由
sigev_notify_attributes指定, 传NULL指针表示使用默认属性.

* mq_notify04.c 启动一个新线程的mq_notify


###Posix实时信号
---

信号可划分为两个大组
1. 其值在SIGRTMIN和SIGRTMAX(包括两者在内)的实时信号
2. 所有其他信号: SIGALARM, SIGINT, SIGKILL, 等等

只有在sigaction调用中指定了SA_SIGINFO, 并且是对SIGRTMIN-SIGRTMAX范围内
的信号进行处理时, 实时行为才有保证

####实时行为隐含特征：

* 信号是排队的.如果一个信号产生了3次, 它就递交3次.
* 当有多个SIGRTMIN-SIGRTMAX范围内的解阻塞信号排队时, **值较小**的信号先于值较大的信
  号递交.（demo signal_realtime.c 中对此进行了验证）
* 当某个非实时信号递交时, 传递给它的信号处理函数的唯一参数是该信号的值；实时信号则
  携带更多信息.通过设置SA_SIGINFO标志安装的任意实时信号处理函数声明如下：

```
void function(int signo, siginfo_t *info, void *context);

typedef struct
{
    int si_signo;          // same value as *signo* argument
    int si_code;           // SI_{USER, QUEUE, TIMER, ASYNCIO, MEGEQ}
    union sigval si_value; // integer or pointer value from sender
} siginfo_t;

```

* 一些新函数定义成使用实时信号工作.例如sigqueue函数用于代替kill函数向某个进程发送
  一个信号, 该新函数允许发送者虽所发送信号传递一个sigval联合

####实时信号由siginfo_t结构中的si_code来标识如何产生

|信号        |说明                                                 |
|:-----------|:----------------------------------------------------|
|SI_ASYNCIO  |信号由某个异步I/O请求的完成产生, 那些aio_XXX函数     |
|SI_MESGQ    |信号在有一个消息被放置到某个空消息队列时产生         |
|SI_QUEUE    |信号有sigqueue函数发出                               |
|SI_TIMER    |信号由使用timer_settime函数设置的某个定时器的到时产生|
|SI_USER     |信号由kill函数发出.                                 |

如果信号由某个其他事件产生, si_code就会被设置成不同于上面所列的某个值.
siginfo_t结构的si_value成员只有在si_code为SI_ASYNCIO, SI_MESGQ,
SI_QUEUE, SI_TIMER时才有效.

* signal_realtime.c 演示实时信号的简单测试程序.由父进程连续触发9个信号, 信号值从da
  到小触发, 每个信号伴随一个int型参数.此示例证明了实时信号是FIFO的, 并且较小值的信
  号优先递交


