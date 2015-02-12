互斥锁和条件变量
===

互斥锁和条件变量总是可用来同步一个进程内的各个线程。但是如果一个互斥锁或条件变量是
存放在多个进程间共享的某个内存区内，那么它还可以被用来同步这几个进程。

互斥锁指代相互排斥，用于保护临界区，以保证任何时刻**只有一个线程/进程**在执行
其中的代码。保护一个临界区的代码的通常轮廓大体如下：

```cpp
lock_the_mutex(...)
操作临界区
unlock_the_mutex(...)
```

**互斥锁的初始化**：

1. 静态分配。 static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
2. 动态分配。 在运行之时通过调用pthread_mutex_init函数来初始化。

**API**

```cpp
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mptr);  // block
int pthread_mutex_trylock(pthread_mutex_t *mptr);  // unblock
int pthread_mutex_unlock(pthread_mutex_t *mptr);
返回: 成功返回0，失败返回Exxx错误码

如果尝试给一个已由另外某个线程锁住的互斥锁上锁，那么pthread_mutex_lock会阻塞到该
互斥锁被解锁为止，pthread_mutex_trylock是对应的非阻塞函数，如果该互斥锁已锁住，
它就返回一个EBUSY错误

```

互斥锁是协作性锁。**由coder来保证**操作临界区之前获取对应互斥锁。

* prodcons_mutex.c 使用互斥锁的producer-consumer示例

##条件变量

互斥锁用于上锁，条件变量用于等待.

条件变量类型为：pthread_cond_t

```cpp
int pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
int pthread_cond_signal(pthread_cond_t *cptr);
返回: 成功返回0，失败返回Exxx错误码

```

每个条件变量总是有一个互斥锁与之关联

pthread_cond_wait **会原子地**执行2个动作

1. 给互斥锁mptr解锁
2. 把调用线程投入睡眠，直到另外某个线程就本条件变量调用pthread_cond_signal, 
   pthread_cond_wait返回前重新给mptr上锁

**给条件变量发送信号的代码：**

```cpp
struct
{     
    pthread_mutex_t mutex;     
    pthread_cond_t  cond;     
    // vars
} var = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, ...};

pthread_mutex_lock(&var.mutex);
设置条件为真
pthread_cond_signal(&var.cond);
pthread_mutex_unlock(&var.mutex);

调用pthread_cond_signal的线程不必是与之关联的互斥锁的当前属主。但是如果需要可预见的
调度行为，那么调用pthread_cond_signal的线程必须锁住该互斥锁

```


**测试条件并进入睡眠以等待该条件变为真的代码：**

```cpp
pthread_mutex_lock(&var.mutex);
while (条件为假)     
    pthread_cond_wait(&var.cond, &var.mutex);
修改条件
pthread_mutex_unlock(&var.mutex);

```

* prodcons_mutex_cond.c 使用条件变量的producer-consumer示例


###条件变量的定时等待和广播

```cpp
#include <pthread.h>

int pthread_cond_broadcast(pthread_cond_t *cptr);
int pthread_cond_timewait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *abstime);
返回: 成功返回0，失败返回Exxx错误码

pthread_cond_timewait 超时返回ETIMEOUT错误

struct timespec
{     
    time_t tv_sec;  // seconds     
    long   tv_nsec; // nanosecondes
};
abstime是UTC时间，而不是时间差
```

##互斥锁和条件变量的属性

```cpp
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mptr, const pthread_mutexattr_t *attr);
int pthread_mutex_destory(pthread_mutex_t *mptr);
int pthread_cond_init(pthread_cond_t *cptr, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cptr);
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);
返回: 成功返回0，失败返回Exxx错误码

```

一旦某个互斥锁属性对象或者某个条件变量属性对象已被初始化，就通过调用不同函数启用或
禁止特定的特性。例如，指定互斥锁或条件变量在不同进程间共享，而不是只在单个进程内的
不同线程间共享

```cpp
#include <pthread.h>

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *ptr, int *valptr);
int pthread_mutexattr_setpshared(const pthread_mutexattr_t *ptr, int val);
int pthread_condattr_getpshared(const pthread_condattr_t *ptr, int *valptr);
int pthread_condattr_setpshared(const pthread_condattr_t *ptr, int val);
返回: 成功返回0，失败返回Exxx错误码

```

这个特性只在<unistd.h&gt;中定义了**_POSIX_THREAD_PROCESS_SHARED**时才得以支持，下面给
出初始化一个互斥锁以便它能在进程间共享的示例：

```cpp
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


