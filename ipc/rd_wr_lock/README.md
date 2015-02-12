##读写锁
===

读写锁是作为pthread_rwlock_t数据类型的变量在内存中分配的。
当读写锁是在单个进程内的各个线程间共享时，这些变量可以定义在那个进程内；
当读写锁是在共享某个内存区的进程间共享时，这些变量应该定义在该共享内存区中。

###读写锁的分配原则
---

1. 只要没有线程持有某个给定的读写锁用于写，那么任意数目的线程才可以持有该读写锁用于读
2. 仅当没有线程持有某个给定的读写锁用于读或写时，才能分配该读写锁用于写。

某些应用中读数据比修改数据频繁，这些应用可以从改用读写锁代替互斥锁中获益。
任意给定时刻允许多个读出者存在提供了更高的并发度，同时在某个写入者修改数
据期间保护该数据，以免任何其他读出者或写入者的干扰。

读写锁的一个日常类比是访问银行账户。

```cpp
#include <pthread.h>

int pthread_rwlock_rdlock(pthread_rwlock_t *rwptr); //获取一个读出锁
int pthread_rwlock_wrlock(pthread_rwlock_t *rwptr); //获取一个写入锁
int pthread_rwlock_unlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwptr);
返回: 成功返回0，失败返回Exxx错误码

```

###读写锁属性
---

1. 静态分配的读写锁，可以用常值PTHREAD_RWLOCK_INITIALIZER来初始化
2. 调用pthread_rwlock_init来动态初始化
3. 当一个线程不再需要某个读写锁时，可以调用pthread_rwlock_destroy来销毁它

```cpp
#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *rwptr, const pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwptr);
返回: 成功返回0，失败返回Exxx错误码

```

数据类型为pthread_rwlockattr_t的某个属性对象一旦初始化，可以通过调用不同的函数来启用
或禁止特定属性。当前定义了的唯一属性是PTHREAD_PROCESS_SHARED。

```cpp
#include <pthread.h>

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, int *valptr);
int pthread_rwlockattr_setpshared(const pthread_rwlockattr_t *attr, int value);
返回: 成功返回0，失败返回Exxx错误码

```

value的值可以为PTHREAD_PROCESS_PRIVATE或PTHREAD_PROCESS_SHARED

###线程清理处理程序
---

有时候，线程会在持有锁期间被动（被其他线程cancel）或主动终止（主动调用pthread_exit）。
为了处理这种情况，线程可以安装或删除清理处理程序

```cpp
#include <pthread.h>

void pthread_cleanup_push(void (*function)(void *), void *arg);

// 删除调用线程的取消清理栈中位于栈顶的函数。如果excute不为0，就调用该函数
void pthread_cleanup_pop(int execute);
返回: 成功返回0，失败返回Exxx错误码

```

###小结
---

与普通的互斥锁相比，当被保护数据的读访问比写访问更为频繁时，读写锁能够提供更高的并发度。


