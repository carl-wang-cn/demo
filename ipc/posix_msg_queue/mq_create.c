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

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)

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

    mq_close(mqd);

    return 0;
}
