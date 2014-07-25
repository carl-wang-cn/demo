
Posix消息队列
==============

###基本特性   

1. 在某个进程往一个队列写入消息之前，并不需要另外某个进程在该队列上等待消息的到达。这个管道和FIFO是相反的，对后两者来说，除非读出者已存在，否则先有写入者是没有意义的。
2. 消息队列具有随内核的持续性。一个进程可以往某个队列写入一些消息，然后终止，再让另外一个进程在以后某个时刻读出这些消息
3. 队列属性   
```
struct mq_attr   
{      
    long mq_flags;
    long mq_maxmsg;      
    long mq_msgsize;      
    long mq_curmsgs;   
}   
```
* mq\_setattr 只能设置flags属性，用于设置或清除阻塞标志。
ma\_maxmsg, mq\_msgsize只能在mq\_open的时候设置，这两个属性必须同时指定。
mq\_curmsgs只能获取，不能设置   
* mq\_getattr用于获取队列的属性   
* mq\_receive总是返回队列中**最高优先级**的**最早**消息

###Posix消息队列的异步事件通知
Posix消息队列允许异步事件通知，以告知何时有一个消息放置到了某个空消息队列中。有两种方式可供选择

1. 产生一个信号
2. 创建一个线程来执行一个指定的函数

```
int mq_notify(mqd_t mqdes, const struct sigevent *notification)   
union sigval   
{
    int sival_int; //integer value       
    void *sival_ptr; //pointer value   
}   
struct sigevent   
{       
    int sigev_notify; // SIGEV_{NONE, SIGNAL, THREAD}       
    union sigval sigev\_value // passed to signal handler or thread                    
    // following two if SIGEV_THREAD       
    void (*sigev_notify_function)(union sigval);       
    pthread_attr_t *sigev_notify_attributes;
}
```

####该函数的若干使用规则：

1. 如果notification参数非空，那么当前进程希望在有一个消息到达所指定的先前为空的队列时得到通知。（我们说该进程被注册为接收该队列的通知）
2. 如果notification参数为NULL， 而且当前进程目前被注册为接收所指定队列的通知，那么已存在的注册将被撤销
3. 任意时刻只有一个进程可以被注册为接收某个给定队列的通知
4. 当有一个消息到达某个先前为空的队列，而且已有一个进程被注册为接受该队列的通知时，只有在没有任何线程阻塞在该队列的mq\_receive调用中的前提下，通知才会发出。也就是说，在mq\_receive调用中的阻塞比任何通知的注册都优先
5. 当该通知被发送给它的注册进程时，其注册即被撤销。该进程必须再次调用mq\_notify以重新注册。

###Posix实时信号实时行为有保证：
只有在sigaction调用中指定了SA\_SIGINFO，并且是对SIGRTMIN-SIGRTMAX范围内的信号进行处理时

####实时行为隐含特征：

* 信号是排队的。如果一个信号产生了3次，它就递交3次。
* 当有多个SIGRTMIN-SIGRTMAX范围内的解阻塞信号排队时，**值较小**的信号先于值较大的信号递交。（demo signal\_realtime中对此进行了验证）
* 当某个非实时信号递交时，传递给它的信号处理函数的唯一参数是该信号的值；实时信号则携带更多信息。通过设置SA\_SIGINFO标志安装的任意实时信号处理函数声明如下：
```
void function(int signo, siginfo_t *info, void *context);     
typedef struct     
{          
        int si_signo; // same value as *signo* argument          
        int si_code;  // SI_{USER, QUEUE, TIMER, ASYNCIO, MEGEQ}          
        union sigval si_value; // integer or pointer value from sender     
} siginfo_t;
```
* 一些新函数定义成使用实时信号工作。

####实时信号由siginfo\_t结构中的si\_code来标识如何产生

1. SI\_ASYNCIO  信号由某个异步I/O请求的完成产生，那些aio\_XXX函数
2. SI\_MESGQ    信号在有一个消息被放置到某个空消息队列时产生
3. SI\_QUEUE    信号有sigqueue函数发出
4. SI\_TIMER    信号由使用timer\_settime函数设置的某个定时器的到时产生
5. SI\_USER     信号由kill函数发出。
6. 如果信号由某个其他事件产生，si\_code就会被设置成不同于上面所列的某个值。siginfo\_t结构的si\_value成员只有在si\_code为SI\_ASYNCIO, SI\_MESGQ, SI\_QUEUE, SI\_TIMER时才有效。


