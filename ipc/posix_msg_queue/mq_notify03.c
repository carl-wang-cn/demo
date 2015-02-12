#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

int pipefd[2];
static void sig_usr1(int);

int main(int argc, char **argv)
{
    int nfds = 0;
    char c;
    fd_set rset;
    mqd_t mqd;
    void *buff = NULL;
    ssize_t n;
    struct mq_attr attr;
    struct sigevent sigev;
    int ret = 0;

    if (2 != argc)
    {
        fprintf(stderr, "usage: %s <name>\n", argv[0]);
        exit(EXIT_FAILURE);
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
    assert(buff);

    signal(SIGUSR1, sig_usr1);
    sigev.sigev_notify= SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    ret = mq_notify(mqd, &sigev);
    if (-1 == ret)
    {
        perror("mq_notify err");
        exit(-1);
    }

    FD_ZERO(&rset);
    for(;;)
    {
        FD_SET(pipefd[0], &rset);
        nfds = select(pipefd[0]+1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(pipefd[0], &rset))
        {
            read(pipefd[0], &c, 1);
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
        }
    }

    return 0;
}

static void sig_usr1(int signo)
{
    write(pipefd[1], "", 1);
    return;
}

