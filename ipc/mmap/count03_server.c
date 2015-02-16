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
    int fd;
    sem_t *mutex;
    int *ptr = NULL;

    if (2 != argc)
    {
        fprintf(stderr, "usage: %s <shmname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shm_unlink(argv[1]);
    // open file, initialize to 0, map into memory
    fd = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, FILE_MODE);
    ftruncate(fd, sizeof(int));

    // 1. mmap 2. fork
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    sem_unlink(SEM_NAME);
    // create, initialize and unlink semaphore
    mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, FILE_MODE, 1);
    sem_close(mutex);

    exit(EXIT_SUCCESS);
}

