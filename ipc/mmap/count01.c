#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "../unpipc.h"


#define SEM_NAME "mysem"

int main(int argc, char **argv)
{
    int fd, i, nloop, zero = 0;
    int *ptr = NULL;
    sem_t *mutex;

    if (3 != argc)
    {
        fprintf(stderr, "usage: %s <pathname> <#loops>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    nloop = atoi(argv[2]);

    // open file, initialize to 0, map into memory
    fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE);
    write(fd, &zero, sizeof(int));

    // 1. mmap 2. fork
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // create, initialize and unlink semaphore
    mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, FILE_MODE, 1);
    sem_unlink(SEM_NAME);

    setbuf(stdout, NULL); // stdout is unbuffered
    if (0 == fork())
    {
        for (i = 0; i < nloop; ++i)
        {
            sem_wait(mutex);
            // 这里直接操作内存, 不再需要read, write这些操作
            printf("child: %d\n", (*ptr)++);
            sem_post(mutex);
        }
        exit(EXIT_SUCCESS);
    }
    
    for (i = 0; i < nloop; ++i)
    {
        sem_wait(mutex);
        printf("parent: %d\n", (*ptr)++);
        sem_post(mutex);
    }
    exit(EXIT_SUCCESS);
}

