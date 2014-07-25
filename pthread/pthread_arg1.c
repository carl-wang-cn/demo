#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 8
char *message[NUM_THREADS];

void *PrintHello(void *threadid)
{
    int *id_ptr, taskid;
    
    sleep(1);
    id_ptr = (int *)threadid;
    taskid = *id_ptr;
    printf("Thread %d: %s\n", taskid, message[taskid]);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];
    int rc, t;

    message[0] = "English: Hello World!";
    message[1] = "French: Bonjour, le monde!";
    message[2] = "Spanish: Hola al mundo";
    message[3] = "Klingon: Nuq neH!";
    message[4] = "German: Guten Tag, Welt!"; 
    message[5] = "Russian: Zdravstvytye, mir!";
    message[6] = "Japan: Sekai e konnichiwa!";
    message[7] = "Latin: Orbis, te saluto!";

    for (t=0; t<NUM_THREADS; t++) {
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        printf("In main: creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)taskids[t]);
        if (0 != rc) {
            printf("ERROR, return code from pthread_create{} is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}

