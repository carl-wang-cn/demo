#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../unpipc.h"


int main(int argc, char **argv)
{
    if (2 != argc)
    {
        fprintf(stderr, "usage: %s <name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shm_unlink(argv[1]);

    exit(EXIT_SUCCESS);
}

