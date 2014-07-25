#include <unistd.h>
#include <fcntl.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)

void my_lock(int fd)
{
    struct flock lock;

    // 这种用法是错误的，Posix只是定义了一个struct中的必须成员，各个实现可以以
    // 任意顺序来排列这些成员，还可以增设特定于实现的成员
    //lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // write lock entire file
    
    fcntl(fd, F_SETLKW, &lock);

    return;
}

void my_unlock(int fd)
{
    struct flock lock;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // unlock entire file

    fcntl(fd, F_SETLK, &lock);

    return;
}

int main(int argc, char **argv)
{
    return 0;
}
