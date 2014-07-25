#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (2 != argc)
    {
        fprintf(stderr, "usage: mq_unlink <queue_name>");
        exit(-1);
    }

    mq_unlink(argv[1]);
     
    return 0;
}
