// 从文件data.txt中读出数据，写入到FIFO中

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../unpipc.h"


int main(int argc, char **argv)
{
    int write_fd = -1;
    int read_fd = -1;
    int res = 0;
    int bytes_sent = 0;
    char buffer[PIPE_BUF+1] = {};

    res = mkfifo(FIFO, FILE_MODE);
    if (res != 0 && EEXIST != errno)
    {
        fprintf(stderr, "could not create fifo %s\n", FIFO);
        exit(EXIT_FAILURE);
    }
    
    printf("process %d opening fifo w_wronly\n", getpid());

    write_fd = open(FIFO, O_WRONLY);
    read_fd = open("data.txt", O_RDONLY);
    printf("process %d result: writefd=%d, readfd=%d\n", getpid(), write_fd, read_fd);

    if (write_fd != -1)
    {
        int bytes_read = 0;
        bytes_read = read(read_fd, buffer, PIPE_BUF);
        printf("read from %s :%s\n", "data.txt", buffer);
        while (bytes_read > 0)
        {
            buffer[bytes_read] = 0;
            res = write(write_fd, buffer, bytes_read);
            if (res == -1)
            {
                fprintf(stderr, "write error on pipe\n");
                exit(EXIT_FAILURE);
            }

            bytes_sent += res;
            memset(buffer, 0, sizeof(buffer));
            bytes_read = read(read_fd, buffer, PIPE_BUF);
            printf("read from %s :[%d]%s\n", "data.txt", bytes_read, buffer);
        }
        close(write_fd);
        close(read_fd);
    }
    else
    {
        fprintf(stderr, "write_fd == -1\n");
        exit(EXIT_FAILURE);
    }

    printf("process %d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
