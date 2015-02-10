/*
 匿名管道，通常是用于父子进程间的通信，半双工模式。如果需要全双工模式，必须创建两个管道
 field[0] 用来读，field[1] 用来写
 注意：有的系统，pipe提供的是全双工管道，但现实中通常不会这么用
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    int data_processed = 0;
    int fileds[2] = {};
    const char data[] = "Hello pipe";
    char buffer[BUFSIZ + 1] = {};
    pid_t pid;

    if (pipe(fileds) == 0)
    {
        pid = fork();
        switch (pid)
        {
            case -1:
                fprintf(stderr, "Fork failure");
                exit(EXIT_FAILURE);
            case 0:
                close(fileds[1]);
                data_processed = read(fileds[0], buffer, BUFSIZ);
                printf("read %d bytes: %s\n", data_processed, buffer);
                exit(EXIT_SUCCESS);
            default:
                close(fileds[0]);
                data_processed = write(fileds[1], data, strlen(data));
                printf("wrote %d bytes: %s\n", data_processed, data);
                sleep(2);
                exit(EXIT_SUCCESS);
        }
    }

    exit(EXIT_FAILURE);
}

