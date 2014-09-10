#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)
#define NBUFF 10

typedef struct
{
    int buff[NBUFF];
    sem_t mutex;
    sem_t nempty;
    sem_t nstored;
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
    sem_init(&shared.mutex, 0, 1);
    sem_init(&shared.nempty, 0, NBUFF);
    sem_init(&shared.nstored, 0, 0);

    // create one producer and one consumer thread
    pthread_create(&tid_produce, NULL, produce, NULL);
    pthread_create(&tid_consume, NULL, consume, NULL);

    // wait for the two threads
    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);

    // remove the semaphores
    sem_destroy(&shared.mutex);
    sem_destroy(&shared.nempty);
    sem_destroy(&shared.nstored);

    return 0;
}

void *produce(void *arg)
{
    int i = 0;

    for (i=0; i<nitems; ++i)
    {
        sem_wait(&shared.nempty); // wait for at least 1 empty slot
        sem_wait(&shared.mutex);
        shared.buff[i%NBUFF] = i;
        sem_post(&shared.mutex);
        sem_post(&shared.nstored); // 1 more stored item
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
        sem_wait(&shared.nstored); // wait for at least 1 stored item
        sem_wait(&shared.mutex);
        if (shared.buff[i % NBUFF] != i)
        {
            printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        }
        sem_post(&shared.mutex);
        sem_post(&shared.nempty); // 1 more empty slot
    }

    return NULL;
}

