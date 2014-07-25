#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>



int main(int argc, char **argv)
{
    mqd_t mqd;
    void  *ptr = NULL;
    size_t len;
    unsigned int prio;
    int ret = 0;

    if (argc != 4)
    {
        fprintf(stdout, "usage: mq_send <name> <#byte> <priority>\n"); //优先级不能高于MQ_PRIO_MAX的值
        exit(-1);
    }

    len = atoi(argv[2]);
    prio = atoi(argv[3]);

    mqd = mq_open(argv[1], O_WRONLY);
    if (-1 == mqd)
    {
        fprintf(stdout, "mq_open error:%d\n", errno);
    }

    ptr = calloc(len, sizeof(char));
    if (NULL == ptr)
    {
        fprintf(stdout, "calloc error\n");
        exit(-1);
    }

    ret = mq_send(mqd, ptr, len, prio);
    if (-1 == ret)
    {
        fprintf(stdout, "mq_send error:%d\n", errno);
        exit(-1);
    }

    return 0;
}
