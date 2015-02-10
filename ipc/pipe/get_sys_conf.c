#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    if (2 != argc)
    {
        fprintf(stderr, "pls usage %s <path_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("PIPE_BUF = %ld, OPEN_MAX = %ld\n",
            pathconf(argv[1], _PC_PIPE_BUF), sysconf(_SC_OPEN_MAX));

    return 0;
}

