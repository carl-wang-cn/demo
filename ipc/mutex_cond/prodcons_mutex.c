#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include "../unpipc.h"


int nitems;  // read-only by producer and consumer

struct 
{
    pthread_mutex_t mutex;
    int buff[MAXNITEMS];
    int nput; // next index to store
    int nval; // next value to store
} shared = {PTHREAD_MUTEX_INITIALIZER};

void *produce(void *);
void *consume(void *);

int main(int argc, char **argv)
{
    int i, nthreads, count[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume;

    if (argc != 3)
    {
        fprintf(stdout, "usage: prodcons_mutex <#items> <#threads>\n");
        exit(-1);
    }

    nitems = MIN(atoi(argv[1]), MAXNITEMS);
    nthreads = MIN(atoi(argv[2]), MAXNTHREADS);

    // start all producers and one consumer
    for (i=0; i<nthreads; i++)
    {
        count[i] = 0;
        pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }
    pthread_create(&tid_consume, NULL, consume, NULL);

    // wait for all producers and the consumer
    for (i=0; i<nthreads; i++)
    {
        pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }
    pthread_join(tid_consume, NULL);

    exit(0);
}

void *produce(void *arg)
{
    for (;;)
    {
        pthread_mutex_lock(&shared.mutex);
        if (shared.nput >= nitems)
        {
            pthread_mutex_unlock(&shared.mutex);
            return NULL; // array is full ,we're done
        }

        shared.buff[shared.nput] = shared.nval;
        shared.nput++;
        shared.nval++;
        pthread_mutex_unlock(&shared.mutex);
        *((int *)arg) += 1;
    }
}

static void consume_wait(int i)
{
    for (;;)
    {
        pthread_mutex_lock(&shared.mutex);
        if (i < shared.nput)
        {
            pthread_mutex_unlock(&shared.mutex);
            return ; // an item is ready
        }
        // 这里的lock->unlock->lock...被称为轮询，也是对CPU的浪费
        // 这里用条件变量会更好，它允许一个线程睡眠到某个事件发生
        // 为止
        pthread_mutex_unlock(&shared.mutex);
    }
}

void *consume(void *arg)
{
    int i;
    for (i=0; i<nitems; i++)
    {
        consume_wait(i); // 等待生产者产生了第i个条目
        if (shared.buff[i] != i)
        {
            printf("buff[%d] = %d\n", i, shared.buff[i]);
        }
    }
    return NULL;
}

