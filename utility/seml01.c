#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

static int sem_id = 0;
static int set_semvalue();
static void del_semvalue();
static int semaphore_p();
static int semaphore_v();

int main(int argc, char **argv)
{
    char message = 'X';
    int i = 0;

    sem_id = semget((key_t)1234, 1, 0666|IPC_CREAT);

    if (argc > 1)
    {
        if (!set_semvalue())
        {
            fprintf(stderr, "failed to initialize semaphore\n");
            exit(EXIT_FAILURE);
        }

        message = argv[1][0];
        sleep(1);
    }

    for (i=0; i<10; i++)
    {
        if (!semaphore_p())
            exit(EXIT_FAILURE);
        printf("%c", message);
        fflush(stdout);
        sleep(2);
        printf("%c", message);
        fflush(stdout);
        if (!semaphore_v())
            exit(EXIT_FAILURE);
        sleep(2);
    }

    sleep(10);
    printf("\n%d - finished\n", getpid());

    if (argc > 1)
    {
        sleep(3);
        del_semvalue();
    }

    exit(EXIT_SUCCESS);
}

static int set_semvalue()
{
    union semun sem_union = {};
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
    {
        return 0;
    }
    return 1;
}

static void del_semvalue()
{
    union semun sem_union = {};
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
    {
        fprintf(stderr, "failed to delete semaphore\n");
    }
}

static int semaphore_p()
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;//除非使用一组信号量，否则它为0
    sem_b.sem_op = -1;//信号量在一次操作中需要改变的数据， -1，即P操作， +1，即V操作
    sem_b.sem_flg = SEM_UNDO;//是操作系统跟踪信号，在进程没有释放信号量而终止时，操作系统释放信号量
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
    return 1;
}

static int semaphore_v()
{
    struct sembuf sem_v;
    sem_v.sem_num = 0;
    sem_v.sem_op = 1;
    sem_v.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_v, 1) == -1)
    {
        fprintf(stderr, "semaphore_v failed\n");
        return 0;
    }
    return 1;
    
}
