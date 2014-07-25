/*
 FIFO指代先进先出，是一个单向数据流（半双工）。每一个FIFO有一个路径名与之关联，从而允许无亲缘关系的进程访问同一个FIFO。
 FIFO也称为有名管道（named pipe）
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)

int main()
{
    const char *fifo_name = "/tmp/my_fifo";
    int read_fd = -1;
    int write_fd = -1;
    int res = 0;
    char buffer[PIPE_BUF+1] = {};
    int bytes_read = 0;
    int bytes_write = 0;

    res = mkfifo(fifo_name, FILE_MODE);
    if (res < 0 && errno != EEXIST)
    {
        fprintf(stderr, "mkfifo error: %d, %s\n", errno, fifo_name);
        exit(0);
    }

    printf("process %d opening fifo o_rdonly\n", getpid());
    read_fd = open(fifo_name, O_RDONLY);
    write_fd = open("datafromfifo.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    printf("process %d result: readfd=%d, writefd=%d\n", getpid(), read_fd, write_fd);

    if (read_fd != -1)
    {
        do
        {
            memset(buffer, 0, sizeof(buffer));
            res = read(read_fd, buffer, PIPE_BUF);
            printf("read from FIFO %s: [%d]%s\n", fifo_name, res, buffer);
            if (res > 0)
            {
                bytes_write = write(write_fd, buffer, res);
                bytes_read += res;
            }
        } while (res > 0);
        close(read_fd);
        close(write_fd);
    }
    else
    {
        fprintf(stderr, "read_fd == -1\n");
        exit(EXIT_FAILURE);
    }
    
    printf("process %d finished, %d bytes read\n", getpid(), bytes_read);
    exit(EXIT_SUCCESS);
}
