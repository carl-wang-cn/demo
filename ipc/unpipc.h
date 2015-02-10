#ifndef UNPIPC_H
#define UNPIPC_H

#include <sys/stat.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) // default permissions for new files

#define FIFO "/tmp/my_fifo"



#endif
