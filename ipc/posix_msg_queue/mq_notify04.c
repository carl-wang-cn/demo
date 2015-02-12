/*
 使用线程来处理异步事件通知
 把sigev_notify设置成SIGEV_THREAD
 该线程调用由sigev_notify_function指定的函数，所用的参数由sigev_value指定，新线程的属性由sigev_notify_attributes
 指定，NULL表示默认属性。
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>


mqd_t mqd;
struct mq_attr attr;
struct sigevent sigev;

static void notify_thread(union sigval); // our thread function

int main(int argc, char **argv)
{
    int ret = 0;

    if (argc != 2)
    {
        fprintf(stdout, "usage: mq_notify03 <name>\n");
        exit(-1);
    }

    mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
    if (-1 == mqd)
    {
        perror("mq_open error");
        exit(-1);
    }
    ret = mq_getattr(mqd, &attr);
    if (-1 == ret)
    {
        perror("mq_getattr error");
        exit(-1);
    }

    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_value.sival_ptr = NULL;
    sigev.sigev_notify_function = notify_thread;
    sigev.sigev_notify_attributes = NULL;

    ret = mq_notify(mqd, &sigev);
    if (-1 == ret)
    {
        perror("mq_notify err");
        exit(-1);
    }

    for (;;)
    {
        pause();
    }

    exit(0);
}

static void notify_thread(union sigval arg)
{
    ssize_t n;
    void *buff = NULL;

    printf("notify_thread started\n");
    buff = malloc(attr.mq_msgsize);
    assert(NULL != buff);

    while ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0)
    {
        printf("read %ld bytes\n", (long)n);
    }
    if (errno != EAGAIN) // 表示暂无消息可读，属于正常现象
    {
        perror("mq_receive error");
        exit(-1);
    }

    free(buff);buff = NULL;
    pthread_exit(NULL);
    
}

