#ifndef _SHMDATA_H
#define _SHMDATA_H

#define TEXT_SZ 2048

struct shared_use_st
{
    int written; //作为一个标志，非0表示可读，0表示可写
    char text[TEXT_SZ]; //记录写入和读取的文本
};

#endif

