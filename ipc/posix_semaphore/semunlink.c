#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "../unpipc.h"


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: semunlink <name>\n");
        return -1;
    }

    sem_unlink(argv[1]);

    return 0;
}

