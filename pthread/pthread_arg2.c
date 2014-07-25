/******************************************************************
 use a struct to pass multiple arguments
*******************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 8
char *message[NUM_THREADS];

struct thread_data {
    int thread_id;
    int sum;
    char *message;
};

struct thread_data thread_data_array[NUM_THREADS];

void *PrintHello(void *threadarg)
{
    int taskid, sum;
    char *hello_msg;
    struct thread_data *my_data;
    
    sleep(1);
    my_data = (struct thread_data *)threadarg;
    taskid = my_data->thread_id;
    sum = my_data->sum;
    hello_msg = my_data->message;

    printf("Thread %d: %s  sum=%d\n", taskid, hello_msg, sum);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];
    int rc, t, sum;

    sum = 0;
    message[0] = "English: Hello World!";
    message[1] = "French: Bonjour, le monde!";
    message[2] = "Spanish: Hola al mundo";
    message[3] = "Klingon: Nuq neH!";
    message[4] = "German: Guten Tag, Welt!"; 
    message[5] = "Russian: Zdravstvytye, mir!";
    message[6] = "Japan: Sekai e konnichiwa!";
    message[7] = "Latin: Orbis, te saluto!";

    for (t=0; t<NUM_THREADS; t++) {
        sum += t;
        thread_data_array[t].thread_id = t;
        thread_data_array[t].sum = sum;
        thread_data_array[t].message = message[t];
        printf("In main: creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)&thread_data_array[t]);
        if (0 != rc) {
            printf("ERROR, return code from pthread_create{} is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}

