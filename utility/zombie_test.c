#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("error\n");
    }
    else if (pid == 0) //child process
    {
        exit(0);
    }
    else //parent process
    {
        sleep(30);
        wait(NULL); //recycle the zombie process
    }

    return 0;
}
