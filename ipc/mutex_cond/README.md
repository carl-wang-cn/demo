互斥锁和条件变量
=================

互斥锁指代相互排斥，用于保护临界区，以保证任何时刻**只有一个线程/进程**在执行其中的代码。保护一个临界区的代码的通常轮廓大体如下：
```
lock_the_mutex(...)
临界区
unlock_the_mutex(...)
```
**互斥锁的初始化**：

1. 静态分配。 static pthread\_mutex\_t lock = PTHREAD\_MUTEX\_INITIALIZER;
2. 动态分配。 在运行之时通过调用pthread\_mutex\_init函数来初始化。

互斥锁是协作性锁。**由coder来保证**来操作临界区之前获取对应互斥锁。

##条件变量

互斥锁用于上锁，条件变量用于等待
条件变量类型为：pthread\_cond\_t

```
int pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
int pthread_cond_signal(pthread_cond_t *cptr);
```

每个条件变量总是有一个互斥锁与之关联

pthread\_cond\_wait **会原子地**执行2个动作

1. 给互斥锁mptr解锁
2. 把调用线程投入睡眠，直到另外某个线程就本条件变量调用

pthread\_cond\_signalpthread\_cond\_wait返回前重新给mptr上锁

####给条件变量发送信号的代码：

```
struct
{     
    pthread_mutex_t mutex;     
    pthread_cond_t  cond;     
    // vars
} var = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, ...}
pthread_mutex_lock(&var.mutex);
设置条件为真
pthread_cond_signal(&var.cond);
pthread_mutex_unlock(&var.mutex);
```

####测试条件并进入睡眠以等待该条件变为真的代码：

```
pthread_mutex_lock(&var.mutex);
while (条件为假)     
    pthread_cond_wait(&var.cond, &var.mutex);
修改条件
pthread_mutex_unlock(&var.mutex);
```

###条件变量的定时等待和广播

```
int pthread_cond_broadcast(pthread_cond_t *cptr);
int pthread_cond_timewait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *abstime);
struct timespec
{     
    time_t tv_sec; // seconds     
}
abstime是UTC时间，而不是时间差
```

##互斥锁和条件变量的属性

```
int pthread_mutex_init(pthread_mutex_t *mptr, const pthread_mutexattr_t *attr);
int pthread_mutex_destory(pthread_mutex_t *mptr);
int pthread_cond_init(pthread_cond_t *cptr, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cptr);
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);
```

一旦某个互斥锁属性对象或者某个条件变量属性对象已被初始化，就通过调用不同函数启用或禁止特定的特性。例如，指定互斥锁或条件变量在不同进程间共享，而不是只在单个进程内的不同线程间共享

```
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *ptr, int *valptr);
int pthread_mutexattr_setpshared(const pthread_mutexattr_t *ptr, int val);
int pthread_condattr_getpshared(const pthread_condattr_t *ptr, int *valptr);
int pthread_condattr_setpshared(const pthread_condattr_t *ptr, int val);
```

这个特性只在<unistd.h>中定义了**\_POSIX\_THREAD\_PROCESS\_SHARED**时才得以支持，下面给出初始化一个互斥锁以便它能在进程间共享的示例：

```
pthread_mutex_t *mptr;  // 这个互斥锁要存放在**某块共享内存**中
pthread_mutexattr_t mattr;

mptr = // some value that points to shared 
memorypthread_mutexattr_init(&mattr);  // 初始化属性
#ifdef _POSIX_THREAD_PROCESS_SHARED         
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);  // 设置特定属性
#else     
    #error this implementation does not support _POSIX_THREAD_PROCESS_SHARED
pthread_mutex_init(mptr, &mattr); // 初始化锁
```

##持有锁期间进程终止

* 没有办法让系统在进程终止时自动释放所持有的锁。
* 进程终止时，内核总是自动清理的唯一同步锁类型是：fcntl记录锁 
* 在使用System V信号量时，应用程序可以选择进程终止时内核是否自动清理某个信号量锁。 
* 线程可以注册一个在自己被另一线程取消时要执行的清理处理程序


