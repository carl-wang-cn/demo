##共享内存区
===

共享内存区是IPC形式中**最快**的. 一旦这样的内存区映射到共享它的进程的地址
空间, 这些进程间数据的传递就不再涉及内核. 然而往该共享内存区存放信息或从
中取走信息的进程间通常需要某种形式的同步.

**再次强调**: 默认情况下fork派生的子进程并不与其父进程共享内存区.

mmap函数把一个文件或一个Posix共享内存区对象映射到调用进程的地址空间. 使用该
函数的3个目的:

1. 使用普通文件以提供内存映射I/O
2. 使用特殊文件以提供匿名内存映射
3. 使用shm_open以提供无亲缘关系进程间的Posix共享内存区

```cpp
#include <sys/mman.h>

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
返回: 成功返回被映射区的起始地址, 失败返回MAP_FAILED

addr 可以指定描述符fd应被映射到的进程内空间的起始地址. 它通常被设为NULL, 这
     样告诉内核自己去选择起始地址

len 映射到调用进程地址空间中的字节数, 它从被映射文件开头起第offset个字节处
    开始算. 通常被设置为0

prot 指定内存映射区的保护, 常见值是 PROT_READ | PROT_WRITE
     prot        说明
     PROT_READ   数据可读
     PROT_WRITE  数据可写
     PROT_EXEC   数据可执行
     PROT_NONE   数据不可访问

flags MAP_SHARED或MAP_PRIVATE这两个标志必须指定一个, 并有选择地 | MAP_FIXED. 
      MAP_PRIVATE 调用进程对被映射数据所做的修改只对本进程可见, 而不改变底层
                  支撑对象
      MAP_SHARED  调用进程对被映射数据所做的修改对于共享该对象的所有进程都可
                  见, 而且确实改变了其底层支撑对象
     
      Flags         说明
      MAP_PRIVATE   变动是私自的
      MAP_SHARED    变动是共享的
      MAP_FIXED     准确的解释addr参数

      从移植性上考虑, MAP_FIXED不应被指定. 可移植性的代码应该把addr置为NULL,
      不指定MAP_FIXED

父子进程之间共享内存区的方法之一: 父进程在调用fork之前先指定MAP_SHARED调用mmap.
Posix.1保证父进程中的内存映射关系存留到子进程中.

int munmap(void *addr, size_t len);
返回: 成功返回0, 失败返回-1

从某个进程的地址空间删除一个映射关系. munmap后再访问这些地址将会导致想调用进程
产生一个SIGSEGV信号

如果被映射区是使用MAP_PRIVATE标志映射的, 那么调用进程对它所做的变动都会被丢弃掉.

addr mmap返回的地址
len  映射区的大小

int msync(void *addr, size_t len, int flags);
返回: 成功返回0, 失败返回-1

内核的虚拟内存算法保持内存映射文件(一般在硬盘上)与内存映射区(在内存中)的同步,
前提是它是一个MAP_SHARED内存区. 如果我们修改了处于内存映射到某个文件的内存区中
某个位置的内容, 那么内核将在稍后某个时刻相应的更新文件. 然而有时候希望确信硬盘
上的文件内容与内存映射区中的内容一致, 这就需要调用msync来执行这种同步

addr和len通常指代内存中的整个内存映射区, 不过也可以指定该内存区的一个子集.
flags参数: MS_ASYNC和MS_SYNC必须指定一个, 但不能同时指定
- MS_ASYNC  执行异步写, 一旦写操作由内核排入队列, 立即返回
- MS_SYNC   执行同步写, ........................, 等到写操作完成后才返回
- MS_INVALIDATE 使高速缓存的数据失效. 指定了该flag, 那么与其最终副本不一致的文
            件数据的所有内存中副本都失效, 后续的引用将从文件中取得数据.

Posix还定义了处理内存管理的4个额外函数
- mlockall会使调用进程的整个内存空间常驻内存. munlockall则撤销这种锁定
- mlcok会是调用进程地址空间的某个指定范围常驻内存, 该函数的参数指定了这个范围的
  起始地址以及从该地址算起的字节数. munlock则撤销某个指定内存区的锁定.

```

使用mmap的好处:

使用内存映射文件所得到的奇妙特性是, 所有的I/O都在内核的掩盖下完成, 我们只需编
写存取内存映射区中各个值的代码. 我们决不调用read, write, lseek, 把显示的文件
I/O操作变换成存取内存单元, 这样可以简化我们的代码, 有时候还能改善性能

mmap的另一个用途是在无亲缘关系的进程间提供共享内存区(需指定MAP_SHARED标志).这
种情形下, 所映射文件的实际内容成了被共享内存区的初始内容, 而且这些进程对该共享
内存区所做的任何变动都复制回所映射的文件(以提供随文件系统的持续性).

- count01.c 父子进程给共享内存区的一个计数器加1, 使用有名信号量
- count02.c 父子进程给共享内存区的一个计数器加1, 使用基于内存的信号量

内存映射一个普通文件时, 内存映射区的大小(mmap的第二个参数)通常等于该文件的大小.
但是文件大小和内存映射区大小可以不同. 我们总是能够访问在当前文件大小以内又在内
存映射区以内的那些字节.

- SIGBUS  内存映射区大小 > 文件大小, 访问了超出文件大小的位置但是未超出内存映射区
- SIGSEGV 越界访问内存映射区


###Posix共享内存区
---

两者无亲缘关系进程间共享内存区的方法

1. 内存映射文件. 由open函数打开, 由mmap函数把所得到的描述符映射到当前进程地址空
   间中的一个文件
2. 共享内存区对象. 由shm_open打开一个Posix.1 IPC名字, 所返回的描述符由mmap函数
   映射到当前进程地址空间

```cpp
#include <sys/mman.h>

int shm_open(const char *name, int oflag, mode_t mode);
返回: 成功返回非负描述符, 失败返回-1

oflag  必须含有O_RDONLY或O_RDWR之一, 还可以有O_CREAT, O_EXCL. 如果 O_RDWR | O_TRUNC,
       而且所需的共享内存区对象已经存在, 那么它将被截断为0长度.
mode   指定权限位, 在O_CREAT时必须指定mode, 未指定O_CREAT时可以mode设为0

int shm_unlink(const char *name);
返回: 成功返回0, 失败返回-1

shm_unlink删除一个共享内存区对象的名字. 跟所有其他unlink函数(删除文件系统中一个路径名
的unlink, 删除一个Posix消息队列的mq_unlink, 以及删除一个Posix有名信号量的sem_unlink)
一样, 删除一个名字不会影响对于底层支撑对象的现有引用, 知道对于该对象的引用全部关闭为
止. 删除一个名字仅仅防止后续的open, mq_open或sem_open调用取得成功.

#include <unistd.h>
int ftruncate(int fd, off_t length);
返回: 成功返回0, 失败返回-1

调用ftruncate来指定新创建的共享内存区对象的大小, 或者修改已存在的对象的大小.

- 对于普通文件: 如果该文件大小大于length, 额外的数据就被丢弃掉, 如果该文件的大小小于
  length, 那么该文件是否修改以及其大小是否增长是未定义的. 
  实际上对于一个普通文件, 把它的大小扩展到length字节的可移植办法是:先lseek到偏移为
  length-1处, 然后write 1个字节的数据
- 对于共享内存区对象: 把该对象的大小设置成length字节.

#include <sys/types.h>
#include <sys/stat.h>
int fstat(int fd, struct stat *buf);
返回: 成功返回0, 失败返回-1

struct stat有12个或以上的成员, 但是当fd指代一个共享内存区对象时, 只有4个成员含有信息
struct stat
{
    ...
    mode_t st_mode;  // mode: S_I{RW}{USR, GRP, OTH}
    uid_t  st_uid;   // user id of owner
    gid_t  st_gid;   // group id of owner
    off_t  st_size;  // size in bytes
    ...
};

```

- shmcreate.c 创建一个具有指定大小的Posix共享内存区对象
- shmunlink.c 删除一个共享内存区对象的名字
- shmwrite.c  打开一个共享内存区对象, 填写一个数据模式
- shmread.c   打开一个共享内存区对象, 验证其数据模式

同一个共享内存区对象内存映射到不同进程的地址空间时, 起始地址可以不同.

- count03_server.c 创建并初始化共享内存区和信号量的程序
- count03_client.c 给存放在共享内存区中的一个计数器加1, 测试的时候可以启动多个client
  来观察效果

- cosumer.c 从共享内存区中取得并输出消息
- producer.c 在共享内存区存放消息




