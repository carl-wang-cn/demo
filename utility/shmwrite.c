#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include "shmdata.h"

/*
 *使用共享内存的优点：
 *  进程间通信非常方便，接口简单，数据的共享使得进程间数据不需要传送，而是直接访问内存，加快程序效率，
 *  也不像匿名管道那样要求进程有一定的父子关系
 *缺点：
 *  共享内存没有提供同步的机制，使得我们在使用共享内存进行进程间通信时，要借助于其他的手段来进行进程间的同步工作
 */

int main()
{
    int running = 1;
    void *shm = NULL;
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ+1] = {};//用于保存输入的文本
    int shmid;

    //创建共享内存
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    //将共享内存连接到当前进程的地址空间
    shm = shmat(shmid, (void *)0, 0);
    if (shm == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\n%d memory attached at %X\n", getpid(), (int)shm);

    //设置共享内存
    shared = (struct shared_use_st *)shm;
    while (running) //向共享内存写数据
    {
        //数据还没被读取，则等待数据被读取，不能向共享内存写入文本
        while (shared->written == 1)
        {
            sleep(1);
            printf("waiting ... \n");
        }

        //向共享内存写入数据
        printf("enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->text, buffer, TEXT_SZ);
        //写完数据，设置written是共享内存段可读
        shared->written = 1;
        //输入了end， 退出循环
        if (strncmp(buffer, "end", 3) == 0)
        {
            running = 0;
        }
    }        

    //把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    sleep(2);
    exit(EXIT_SUCCESS);
}
