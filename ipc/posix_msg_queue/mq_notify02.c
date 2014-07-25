/*
 使用sigwait代替信号处理程序的信号通知
 调用sigwait前，我们阻塞某个信号集。我们将这个信号集指定为set参数。sigwait然后一直阻塞到这些信号中有一个或
 多个待处理，这是它返回一个信号（存放在指针sig中）。这个过程称为“同步地等待一个异步事件”

 大多数讨论线程的书，推荐在多线程化的进程中使用sigwait来处理所有信号，而绝不要使用异步信号处理程序
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


int main(int argc, char **argv)
{
    int signo;
    mqd_t mqd;
    void *buff = NULL;
    ssize_t n;
    sigset_t newmask;
    struct mq_attr attr;
    struct sigevent sigev;
    int ret = 0;

    if (argc != 2)
    {
        fprintf(stdout, "usage: mq_notify02 <name>\n");
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
    buff = malloc(attr.mq_msgsize); //buff的空间要不小于attr的msgsize成员，否则mq_receive会返回EMSGSIZE错误

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, NULL); //block SIGUSR1

    sigev.sigev_notify= SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    ret = mq_notify(mqd, &sigev);
    if (-1 == ret)
    {
        perror("mq_notify err");
        exit(-1);
    }

    for (;;)
    {
        ret = sigwait(&newmask, &signo);
        if (0 != ret)
        {
            fprintf(stderr, "sigwait error:%d\n", ret);
            exit(-1);
        }


        if (SIGUSR1 == signo)
        {
            mq_notify(mqd, &sigev);
            while ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0)
            {
                printf("read %ld bytes\n", (long)n);
            }
            if (errno != EAGAIN)
            {
                perror("mq_receive error");
                exit(-1);
            }
        }
    }

    return 0;

}

