#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "../unpipc.h"


struct shared
{
    sem_t mutex; // the mutex: a Posix memory-based semaphore
    int count;
} shared;

int main(int argc, char **argv)
{
    int fd, i, nloop;
    struct shared *ptr;

    if (3 != argc)
    {
        fprintf(stderr, "usage: %s <pathname> <#loops>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    nloop = atoi(argv[2]);

    // open file, initialize to 0, map into memory
    fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE);
    write(fd, &shared, sizeof(struct shared));

    // 1. mmap 2. fork
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // initialize semaphore that is shared between processes
    sem_init(&ptr->mutex, 1, 1);

    setbuf(stdout, NULL);
    if (0 == fork())
    {
        for (i = 0; i < nloop; ++i)
        {
            sem_wait(&ptr->mutex);
            // 这里直接操作内存, 不再需要read, write这些操作
            printf("child: %d\n", ptr->count++);
            sem_post(&ptr->mutex);
        }
        exit(EXIT_SUCCESS);
    }
    
    for (i = 0; i < nloop; ++i)
    {
        sem_wait(&ptr->mutex);
        printf("parent: %d\n", ptr->count++);
        sem_post(&ptr->mutex);
    }
    exit(EXIT_SUCCESS);
}

