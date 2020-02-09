#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (2 != argc)
    {
        fprintf(stderr, "usage: mq_unlink <queue_name>");
        exit(-1);
    }


    // 每个消息队列有一个保存其当前打开着描述符数的引用计数器,
    // 当一个消息队列的引用计数仍大于0时, 可以删除该队列的name, 但是该队列不会析构.
    // 最后一个mq_close后, 再调用mq_unlink, 才会从系统中删除该队列及其上的消息
    mq_unlink(argv[1]);
     
    return 0;
}

