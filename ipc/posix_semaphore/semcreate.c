#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <assert.h>
#include "../unpipc.h"


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
    assert(SEM_FAILED != sem);

    //如果忘记sem_close， 当进程终止时，该信号量也被关闭（所占用的系统资源随之
    //释放），但它的值仍然保持
    sem_close(sem);

    return 0;
}

