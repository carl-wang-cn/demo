/*
 popen函数，FILE *popen(const char *command, const char *type);
 创建一个管道并启动另外一个进程(command命令)，该进程要么从该管
 道读出标准输入（type为"w"），要么往该管道写入标准输出（type 为"r"）
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *read_fp = NULL;
    FILE *write_fp = NULL;
    char buffer[BUFSIZ + 1] = {};
    int chars_read = 0;

    read_fp = popen("ls -l /home/carl", "r");
    write_fp = popen("grep rwxrwxr-x", "w");

    if (read_fp && write_fp)
    {
        chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        while (chars_read)
        {
            buffer[chars_read] = 0;
            printf("read buffer:%s\n", buffer);
            fwrite(buffer, sizeof(char), chars_read, write_fp);
            chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        }

        pclose(read_fp);
        pclose(write_fp);
        exit(EXIT_SUCCESS);
    }

    exit(EXIT_FAILURE);
}
