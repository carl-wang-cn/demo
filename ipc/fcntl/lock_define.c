#include "lock_define.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)
static const char *SEQFILE = "seqno";
#define MAXLINE 80

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    //lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    // 这种用法是错误的，Posix只是定义了一个struct中的必须成员，各个实现可以以
    // 任意顺序来排列这些成员，还可以增设特定于实现的成员

    // 一定要以这种方式来进行初始化操作
    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;
    
    return fcntl(fd, cmd, &lock);
}

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    if (-1 == fcntl(fd, F_SETLK, &lock))
    {
        return -1;
    }

    if (F_UNLCK == lock.l_type)
    {
        return 0; // false, region not locked by another proc
    }

    return lock.l_pid; // true, return positive PID of lock owner
}

