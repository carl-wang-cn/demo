#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    long mq_open_max = sysconf(_SC_MQ_OPEN_MAX);

    // return -1 and errno == 0, means no definite limit
    if (-1 == mq_open_max && 0 != errno)
    {
        printf("errno = %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("MQ_OPEN_MAX = %ld, MQ_PRIO_MAX = %ld\n", mq_open_max,
            sysconf(_SC_MQ_PRIO_MAX));

    return 0;
}
