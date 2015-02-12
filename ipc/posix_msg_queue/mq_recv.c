#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char **argv)
{
    int c, flags, ret;
    mqd_t mqd;
    ssize_t n;
    unsigned int prio;
    void *buff = NULL;
    struct mq_attr attr = {};

    flags = O_RDONLY;
    while ((c = getopt(argc, argv, "n")) != -1)
    {
        switch (c)
        {
        case 'n':
            flags |= O_NONBLOCK;
            break;
        }
    }

    if (optind != argc-1)
    {
        fprintf(stderr, "usage: mq_recv [-n] <name>\n");
        exit(-1);
    }

    mqd = mq_open(argv[optind], flags);
    if (-1 == mqd)
    {
        fprintf(stdout, "mq_open error:%d\n", errno);
    }

    // 这里是要获取attr中的属性msgsize，来确定mq_receive需要开辟的buff大小
    ret = mq_getattr(mqd, &attr);
    if (-1 == ret)
    {
        fprintf(stderr, "mq_getattr error:%d\n", errno);
        exit(-1);
    }
    fprintf(stdout, "max #msgs=%ld, max #bytes/msg=%ld, #currently on queue = %ld\n", 
            attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    buff = malloc(attr.mq_msgsize); //buff的空间要不小于attr的msgsize成员，否则mq_receive会返回EMSGSIZE错误

    // mq_receive总是返回最高优先级的最早消息
    n = mq_receive(mqd, buff, attr.mq_msgsize, &prio);
    if (-1 == n)
    {
        fprintf(stderr, "mq_receive error:%d\n", errno);
        exit(-1);
    }

    printf("read %ld bytes, priority = %u\n", (long)n, prio); //数值越大，优先级越高

    return 0;
}

