#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../../unpipc.h"

#define NBUFF 10

const char *SEM_MUTEX = "mutex";
const char *SEM_NEMPTY = "nempty";
const char *SEM_NSTORED = "nstored";

typedef struct
{
    int buff[NBUFF];
    sem_t *mutex;
    sem_t *nempty;
    sem_t *nstored;
} Shared;

int nitems; // read only by producer and consumer
Shared shared;

void *produce(void *);
void *consume(void *);

int main(int argc, char **argv)
{
    pthread_t tid_produce;
    pthread_t tid_consume;

    if (argc != 2)
    {
        printf("usage: %s <#items>", argv[0]);
        return -1;
    }

    nitems = atoi(argv[1]);

    // create the semaphores
    shared.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, FILE_MODE, 1);
    shared.nempty = sem_open(SEM_NEMPTY, O_CREAT | O_EXCL, FILE_MODE, NBUFF);
    shared.nstored = sem_open(SEM_NSTORED, O_CREAT | O_EXCL, FILE_MODE, 0);

    // create one producer and one consumer thread
    pthread_create(&tid_produce, NULL, produce, NULL);
    pthread_create(&tid_consume, NULL, consume, NULL);

    // wait for the two threads
    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);

    // sem_close will happen automaticly when the process ends
    // so the next 3 sem_close call can be omited
    sem_close(shared.mutex);
    sem_close(shared.nempty);
    sem_close(shared.nstored);

    // remove the semaphores
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_NEMPTY);
    sem_unlink(SEM_NSTORED);

    return 0;
}

void *produce(void *arg)
{
    int i = 0;

    for (i=0; i<nitems; ++i)
    {
        sem_wait(shared.nempty); // wait for at least 1 empty slot
        sem_wait(shared.mutex);
        shared.buff[i%NBUFF] = i;
        sem_post(shared.mutex);
        sem_post(shared.nstored); // 1 more stored item
    }

    return NULL;
}

void *consume(void *arg)
{
    int i = 0;

    for (i=0; i<nitems; ++i)
    {
        // attention!!!
        // the next 2 sem_wait's order can not be exchanged, or it will result in deadlock
        // attention!!!
        sem_wait(shared.nstored); // wait for at least 1 stored item
        sem_wait(shared.mutex);
        if (shared.buff[i % NBUFF] != i)
        {
            printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        }
        sem_post(shared.mutex);
        sem_post(shared.nempty); // 1 more empty slot
    }

    return NULL;
}

