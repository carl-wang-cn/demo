Posix信号量
==========

#####信号量是一种用于提供不同进程间或一个给定进程的不同线程间同步手段的原语。

* Posix有名信号量： 使用Posix IPC名字标识，可用于进程或线程间的同步。
* Posix基于内存的信号量： 存放在共享内存中，可用于进程或线程间的同步。
* System V信号量： 在内核中维护，可用于进程或线程间的同步。

**信号量的值是随内核持续的**


* * *

#####一个进程可以在某个某个信号量上执行的三种操作

1. create一个信号量。这还要求调用者指定初始值，对于二值信号量来说，它通常是1，但也可以是0.
2. wait一个信号量。该操作会测试这个信号量的值，如果其值<=0，那就等待（阻塞），一旦其值变为>0，就将它减1.

    ```cpp
    while (semaphore_value <= 0)
        ; // wait; block the thread or process
    semaphore--;
    // now we have the semaphore
    ```
    **注意：**while语句中测试该信号量的值和其后将它减1（如果该值大于0），这两个步骤必须是原子的

3. post一个信号量。该操作将信号量的值加1，`semaphore_value++`

#####信号量、互斥锁、条件变量的差异

1. 互斥锁必须总是由给它上锁的线程解锁，信号量的post却不必由执行过它的wait操作的同一线程执行。
2. 互斥锁要么被锁住，要么被解开（二值状态，类似于二值信号量）。
3. 既然信号量有一个与之关联的状态（它的计数值），那么信号量的post操作总是被记住。然而当向一个条件变量发送信号时，如果没有线程等待在该条件变量上，那么该信号将丢失。
4. 当持有某个**信号量**的进程没有释放它就终止时，**内核并不给该信号量解锁**。而当持有某个**记录锁**的进程没有释放它就终止时，**内核自动地释放它** 
5. 在各种各样的同步技巧（互斥锁、条件变量、读写锁、信号量）中，能够从信号处理程序中安全调用的唯一函数是`sem_post`

**每种同步方式都有各自的适用场景。互斥锁是为上锁而优化的，条件变量是为等待而优化的，信号量既可用于上锁，也可用于等待，因而可能导致更多的开销和更高的复杂性**

* * *

#####Posix信号量API


`sem_open`创建一个新的有名信号量或打开一个已存在的有名信号量。有名信号量总是既可用于线程间的同步，又可用于进程间的同步

```cpp
#include <semaphore.h>

sem_t *sem_open(const char *name, int oflag, ... /*mode_t mode, usigned int value*/)
                    返回：成功返回指向信号量的指针，出错返回SEM_FAILED
```

* oflag可以为0、O\_CREAT或O\_CREAT|O\_EXCL。如果指定了O\_CREAT标志，那么后面两个参数是需要的：
    * mode指定权限位，
    * value指定信号量初始值。二值信号量为1，计数信号量则往往 >1
* oflag为O\_CREAT时，如果信号量尚未存在，就创建，如果已存在，就打开
* oflag为O\_CREAT|O\_EXCL时，如果信号量已存在，出错

---

使用`sem_open`打开的有名信号量，使用`sem_close`将其关闭

```cpp
#include <semaphore.h>

int sem_close(sem_t *sem);
                    返回：成功返回0，出错返回-1
```

* 一个进程终止时，内核还对其上仍然打开着的所有有名信号量自动执行这样的信号量关闭操作（不论是自愿终止还是非自愿终止）
* 关闭一个信号量并没有将它从系统中删除。Posix有名信号量至少是随内核持续的：即使当前没有进程打开着某个信号量，它的值仍然保持。

---

有名信号量使用`sem_unlink`从系统中删除

```cpp
#include <semaphore.h>

int sem_unlink(const char *name);
                    返回：成功返回0，出错返回-1

```

每个信号量有几个引用计数器记录当前的打开次数，sem\_unlink类似于文件I/O的unlink函数：当引用计数还是大于0时，name就能从文件系统删除，然而其信号量的析构却要等到最后一个sem\_close发生时为止。

---

```cpp
#include <semaphore.h>

int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
                    返回：成功返回0，出错返回-1
```

* sem\_wait测试所指定信号量的值，如果大于0，将它减1并返回。如果等于0，调用线程就被sleep，直到该值变为大于0，这时再将它减1，函数随后返回。 **测试并减1必须是原子**的
* sem\_trywait与sem\_wait类似，不同之处在于，如果所指定的信号量值已经是0，调用线程不会被sleep，函数立即返回，返回EAGAIN错误
* 如果调用被某个信号中断，sem\_wait可能过早地返回，返回错误为EINTR。

---

当一个线程使用完某个信号量时，应该调用`sem_post`，把所指定的信号量的值加1，然后唤醒正在等待该信号量值变为正数的任意线程。

```cpp
#include <semaphore.h>

int sem_post(sem_t *sem);
int sem_getvalue(sem_t *sem, int *valp);
                    返回：成功返回0，出错返回-1
```

* `sem_getvalue`在由`valp`指向的整数中返回所指定信号量的当前值。如果该信号量当前已上锁，那么返回0或某个负数（该负数的绝对值表示等待该信号量解锁的线程数）

* * *

#####示例程序

优于Posix有名信号量至少具有随内核的持续性，因此我们可以**跨多个程序**操纵它们。

```bash
.
├── makefile
├── semcreate.c
├── semgetvalue.c
├── sempost.c
├── semunlink.c
└── semwait.c
```

以上semxxx.c是几个用来操纵信号量的小程序，我们的演示过程如下，演示环境为ubuntu 14.04lts 版本

```bash
carl@localhost:~/demo/ipc/posix_semaphore$ ./semcreate test
carl@localhost:~/demo/ipc/posix_semaphore$ ./semgetvalue test
value = 1
carl@localhost:~/demo/ipc/posix_semaphore$ ./semwait test
pid 2952 has semaphore, value = 0
^C
carl@localhost:~/demo/ipc/posix_semaphore$ ./semgetvalue test
value = 0     # 仍然为0
carl@localhost:~/demo/ipc/posix_semaphore$ ./semwait test &
[1] 2955
carl@localhost:~/demo/ipc/posix_semaphore$ ./semgetvalue test
value = 0     # 本实现不使用负值来表示当前等待该信号量的进程/线程数
carl@localhost:~/demo/ipc/posix_semaphore$ ./semwait test &
[2] 2957
carl@localhost:~/demo/ipc/posix_semaphore$ ./semgetvalue test
value = 0
carl@localhost:~/demo/ipc/posix_semaphore$ ./sempost test
pid 2955 has semaphore, value = 0
value = 0      # 看起来等待着的进程优于挂出信号量的进程运行
carl@localhost:~/demo/ipc/posix_semaphore$ ./sempost test
pid 2957 has semaphore, value = 0
value = 0
```

producer\_consumer01.c使用Posix有名信号量，演示了生产者-消费者问题。

***

**基于内存的信号量**

以上讲述的都是Posix有名信号量，这些信号量由一个name参数标识，它通常指代文件系统中的某个文件。彼此无亲缘关系的不同进程需使用信号量时，通常使用有名信号量。
Posix也提供基于内存的信号量，由应用程序分配信号量的内存空间，然后由系统初始化它们的值。

```cpp
#include <semphore.h>

int sem_init(sem_t *sem, int shared, unsigned int value);
            return: 0 on success, -1 on error

int sem_destroy(sem_t *sem);
            return: 0 on success, -1 on error
```

基于内存的信号量是由`sem_init`初始化的。

* sem参数指向应用**应用程序必须分配**的sem\_t变量。
* shared==0，待初始化的信号量是在统一进程的各个线程间共享
shared!=0，待初始化的信号量是在不同进程间共享。该信号量必须存放在某种类型的共享内存区中，而即将使用它的所有进程都要能访问该共享内存区。
* value参数，是该信号量的初始值

**attention**
对于一个给定的信号量，必须小心保证只调用`semm_init`一次。对一个已初始化过的信号量调用`sem_init`，结果是未定义的。

基于内存的信号量至少具有随进程的持续性，然而它们真正的持续性却取决于存放信号量的内存区的类型。只要含有某个内存信号量的内存区保持有效，该信号量就一直存在。

* 如果某个基于内存的信号量是由单个进程内的各个线程共享的，那么该信号量具有随进程的持续性，当该进程终止时它也消失。
* 如果某个基于内存的信号量是在不同进程间共享的，那么该信号量必须存放在共享内存区中，因而只要该共享内存区仍然存在，该信号量也就继续存在
>服务器可以创建一个共享内存区，在该共享内存区中初始化一个Posix基于内存的信号量，然后终止。一段时间后，一个或多个客户可打开该共享内存区，访问存放在其中的基于内存的信号量。

producer\_consumer02.c使用Posix基于内存的信号量，演示了生产者-消费者问题。

