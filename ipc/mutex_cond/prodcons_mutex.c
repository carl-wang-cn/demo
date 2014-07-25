#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

#define MAXNITEMS   1000000
#define MAXNTHREADS 100

#define min(a, b) (a)>(b)?(b):(a)

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

    nitems = min(atoi(argv[1]), MAXNITEMS);
    nthreads = min(atoi(argv[2]), MAXNTHREADS);

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
        pthread_mutex_unlock(&shared.mutex);
    }
}

void *consume(void *arg)
{
    int i;
    for (i=0; i<nitems; i++)
    {
        consume_wait(i);
        if (shared.buff[i] != i)
        {
            printf("buff[%d] = %d\n", i, shared.buff[i]);
        }
    }
    return NULL;
}
