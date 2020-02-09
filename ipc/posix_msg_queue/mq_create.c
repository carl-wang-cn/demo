/*
 创建一个msg_queue
 编译需要链接 -lrt
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../unpipc.h"

int main(int argc, char **argv)
{
    int flags = O_RDWR | O_CREAT | O_EXCL;
    mqd_t mqd;
    int c = 0;
    struct mq_attr attr = {};

    while ((c = getopt(argc, argv, "em:z:") != -1))
    {
        switch (c)
        {
        case 'e':
            flags |= O_EXCL;
            break;
        case 'm':
            attr.mq_maxmsg = atol(optarg);
            break;
        case 'z':
            attr.mq_msgsize = atol(optarg);
            break;
        }
    }

    if (optind != argc-1)
    {
        fprintf(stderr, "usage: mq_create [-e] [-m maxmsg -z msgsize] <name>\n");
        exit(-1);
    }

    // mq_msgsize and mq_maxmsg must be set at the same time
    if ((attr.mq_maxmsg == 0 && attr.mq_msgsize != 0) || (attr.mq_maxmsg != 0 && attr.mq_msgsize == 0))
    {
        fprintf(stderr, "must specify both -m maxmsg and -z msgsize at the same time");
        exit(-1);
    }

    mqd = mq_open(argv[optind], flags, FILE_MODE, (attr.mq_maxmsg != 0) ? &attr: NULL);
    if (-1 == mqd)
    {
        fprintf(stderr, "mq_open %s error:%d\n", argv[1], errno);
        exit(-1);
    }

    // mq_close后, 调用进程可不再使用该描述符,但其消息队列并不从系统中删除
    // 一个进程终止时，它的所有打开着的消息队列都关闭，就像调用了mq_close一样
    // 要从系统中删除该消息队列, 必须调用mq_unlink
    mq_close(mqd);

    return 0;
}

