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


    // 每个消息队列有一个保存其当前打开着描述符数的引用计数器，当一个消息队列的
    // 引用计数仍大于0时, 其name就能删除，但是该队列的析构要到最后一个mq_close
    // 发生时才进行队列及其上的消息，一直存在到调用mq_unlink并让它的引用计数达
    // 到0以删除该队列为止
    mq_unlink(argv[1]);
     
    return 0;
}

