#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

/*
 与命名管道相比，消息队列的优势：
 1. 消息队列也可以独立于发送和接收进程而存在，从而消除了在同步命名管道的打开和关闭时可能产生的困难
 2. 通过发送消息可以避免命名管道的同步和阻塞问题，不需要由进程自己来提供同步方法
 3. 接收程序可以通过消息类型有选择地接收数据，而不是像命名管道中那样，只能默默地接收。
 */

#define MAX_TEXT 512
struct msg_st
{
    long int msg_type;
    char text[MAX_TEXT];
};

int main()
{
    int running = 1;
    struct msg_st data = {};
    char buffer[BUFSIZ] = {};
    int msgid = -1;

    //建立消息队列
    msgid = msgget((key_t)1234, 0666|IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //向消息队列中写消息，直到写入end
    while (running)
    {
        //输入数据
        printf("enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        data.msg_type = 1;
        strcpy(data.text, buffer);

        //向队列中发送数据
        if (msgsnd(msgid, (void *)&data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        //输入end结束输入
        if (strncmp(buffer, "end", 3) == 0)
        {
            running = 0;
        }
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}
