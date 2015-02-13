#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <assert.h>
#include "../unpipc.h"


int main(int argc, char **argv)
{
    sem_t *sem;
    int val;
    int ret = 0;

    if (argc != 2)
    {
        printf("usage: semwait <name>\n");
        return -1;
    }

    sem = sem_open(argv[1], 0);
    assert(SEM_FAILED != sem);

    sem_wait(sem);
    ret = sem_getvalue(sem, &val);
    assert(-1 != ret);
    printf("pid %ld has semaphore, value = %d\n", (long)getpid(), val);

    pause();
    return 0;
}

