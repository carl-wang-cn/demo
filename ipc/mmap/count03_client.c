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
    int fd, i, nloop;
    pid_t pid;
    int *ptr = NULL;
    sem_t *mutex;

    if (3 != argc)
    {
        fprintf(stderr, "pls usage: %s <shmname> <#nloops>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    nloop = atoi(argv[2]);

    fd = shm_open(argv[1], O_RDWR, FILE_MODE);
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    mutex = sem_open(SEM_NAME, 0);

    pid = getpid();

    for (i = 0; i < nloop; ++i)
    {
        sem_wait(mutex);
        printf("pid %ld: %d\n", (long)pid, (*ptr)++);
        sem_post(mutex);
    }
    exit(EXIT_SUCCESS);
}
