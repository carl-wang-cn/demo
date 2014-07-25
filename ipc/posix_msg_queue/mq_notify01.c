/*
 Posix消息队列允许异步事件通知，以告知何时有一个消息放置到了某个空消息队列中
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t mqflag;  // set nonzero by signal handler
static void sig_usr1(int);

int main(int argc, char **argv)
{
    mqd_t mqd;
    void *buff = NULL;
    ssize_t n;
    sigset_t zeromask, newmask, oldmask;
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

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigemptyset(&oldmask);
    sigaddset(&newmask, SIGUSR1);

    signal(SIGUSR1, sig_usr1);
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
        sigprocmask(SIG_BLOCK, &newmask, &oldmask);
        while (0 == mqflag)
        {
            sigsuspend(&zeromask);
        }
        mqflag = 0; // reset flag

        mq_notify(mqd, &sigev);
        while ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0)
        {
            printf("read %ld bytes\n", (long)n);
        }
        if (errno != EAGAIN) // 表示暂无消息可读，属于正常现象
        {
            perror("mq_receive error");
            exit(-1);
        }
        sigprocmask(SIG_UNBLOCK, &newmask, NULL);
    }

    return 0;

}

static void sig_usr1(int signo)
{
    mqflag = 1;
    return;
}
