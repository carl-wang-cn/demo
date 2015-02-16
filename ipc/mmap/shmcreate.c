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
    int c, fd, flags;
    char *ptr = NULL;
    off_t length;

    flags = O_RDWR | O_CREAT;

    while ((c = getopt(argc, argv, "e")) != -1)
    {
        switch (c)
        {
            case 'e':
                flags |= O_EXCL;
                break;
        }
    }

    if (optind != argc - 2)
    {
        fprintf(stderr, "usage: %s [-e] <name> <length>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    length = atoi(argv[optind + 1]);

    fd = shm_open(argv[optind], flags, FILE_MODE);
    ftruncate(fd, length);

    ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    exit(EXIT_SUCCESS);
}

