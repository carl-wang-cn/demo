#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    long sem_nsems_max = sysconf(_SC_SEM_NSEMS_MAX);

    // return -1 and errno == 0, means no definite limit
    if (-1 == sem_nsems_max)
    {
        printf("sysconf return -1 and errno = %d\n", errno);
        printf("SEM_NSEMS_MAX == -1 means no definite limit\n");
    }

    printf("SEM_NSEMS_MAX = %ld, SEM_VALUE_MAX = %ld\n",
            sem_nsems_max,  sysconf(_SC_SEM_VALUE_MAX));

    return 0;
}
