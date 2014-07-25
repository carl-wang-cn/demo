/****************************************************************************************
 an unsafe way to pass thread arguments. 
 In this case, the argument var is changed by the main thread as it creates new threads
 ***************************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 8
char *message[NUM_THREADS];

void *PrintHello(void *threadid)
{
    int taskid;
    
    sleep(1);
    taskid = (int)threadid;
    printf("Thread %d\n", taskid);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t threads[NUM_THREADS];
    int rc, t;

    for (t=0; t<NUM_THREADS; t++) {
        printf("In main: creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)&t); //var t may be changed before 'PrintHello' use it
        if (0 != rc) {
            printf("ERROR, return code from pthread_create{} is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}

