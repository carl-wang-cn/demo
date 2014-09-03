#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)

int main(int argc, char **argv)
{
    sem_t *sem;
    int val;

    if (argc != 2)
    {
        printf("usage: semwait <name>\n");
        return -1;
    }

    sem = sem_open(argv[1], 0);
    sem_wait(sem);
    sem_getvalue(sem, &val);
    printf("pid %ld has semaphore, value = %d\n", (long)getpid(), val);

    pause();
    return 0;
}
