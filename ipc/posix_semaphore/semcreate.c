#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)

int main(int argc, char **argv)
{
    int c, flags;
    sem_t *sem;
    unsigned int value;

    flags = O_RDWR | O_CREAT;
    value = 1;

    c = getopt(argc, argv, "ei:");
    while (-1 != c)
    {
        switch (c)
        {
            case 'e':
                flags |= O_EXCL;
                break;

            case 'i':
                value  = atoi(optarg);
                break;
        }
    }

    if (optind != (argc - 1))
    {
        printf("usage: semcreate [-e] [-i initalvalue] <name>\n");
        return -1;
    }

    sem = sem_open(argv[optind], flags, FILE_MODE, value);
    sem_close(sem); // 如果忘记sem_close， 当进程终止时，该信号量也被关闭（所占用的系统资源随之释放），但它的值仍然保持

    return 0;
}
