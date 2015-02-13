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
        printf("usage: semgetvalue <name>\n");
        return -1;
    }

    sem = sem_open(argv[1], 0);
    assert(SEM_FAILED != sem);

    ret = sem_getvalue(sem, &val);
    assert(-1 != ret);
    printf("value = %d\n", val);

    return 0;
}

