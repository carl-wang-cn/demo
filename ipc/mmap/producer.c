#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "pro_con.h"
#include "../unpipc.h"



int main(int argc, char **argv)
{
    int fd, i, nloop, nusec;
    pid_t pid;
    char mesg[MESGSIZE];
    long offset;
    struct shmstruct *ptr = NULL;

    if (4 != argc)
    {
        fprintf(stderr, "usage: %s <name> <#nloops> <#usec>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    nloop = atoi(argv[2]);
    nusec = atoi(argv[3]);

    // open and map shared memory that producer must create
    fd = shm_open(argv[1], O_RDWR, FILE_MODE);
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    pid = getpid();
    for (i = 0; i < nloop; ++i)
    {
        usleep(nusec);
        snprintf(mesg, MESGSIZE, "pid %ld: msg: %d\n", (long)pid, i);

        if (sem_trywait(&ptr->nempty) == -1)
        {
            if (EAGAIN == errno)
            {
                sem_wait(&ptr->noverflowmutex);
                ptr->noverflow++;
                sem_post(&ptr->noverflowmutex);
                continue;
            }
            else
            {
                fprintf(stderr, "sem_trywait error\n");
                exit(EXIT_FAILURE);
            }
        }
        sem_wait(&ptr->mutex);
        offset = ptr->msgoff[ptr->nput];
        if (++(ptr->nput) >= NMESG)
        {
            ptr->nput = 0;  // circulate buffer
        }
        sem_post(&ptr->mutex);
        strcpy(&ptr->msgdata[offset], mesg);
        sem_post(&ptr->nstored);
    }

    exit(EXIT_SUCCESS);
}

