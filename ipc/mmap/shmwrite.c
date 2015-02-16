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
    int i, fd;
    struct stat stat;
    unsigned char *ptr = NULL;

    if (2 != argc)
    {
        fprintf(stderr, "usage: %s <name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = shm_open(argv[1], O_RDWR, FILE_MODE);
    fstat(fd, &stat);
    
    ptr = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // set: ptr[i] = i
    for (i = 0; i < stat.st_size; ++i)
    {
        *ptr++ = i % 256;
    }

    exit(EXIT_SUCCESS);
}

