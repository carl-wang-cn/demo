#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5


void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for (t=0; t<NUM_THREADS; t++) {
        printf("In main: creating thread %ld\n", t);

        // 第4个参数，如果传&t这样的地址的话，那么在PrintHello里面
        // 输出的tid会全部都一样，观察不到依次创建了5个线程
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (0 != rc) {
            printf("ERROR, return code from pthread_create{} is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}

