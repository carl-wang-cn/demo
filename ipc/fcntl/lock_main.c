#include "lock_define.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_ISGID | S_IROTH)
static const char *SEQFILE = "seqno";
#define MAXLINE 80

int main(int argc, char **argv)
{
    int fd;
    long i, seqno;
    pid_t pid;
    ssize_t n;
    char line[MAXLINE+1] = {};

    pid = getpid();
    fd = open(SEQFILE, O_RDWR, FILE_MODE);

    for (i=0; i<20; ++i)
    {
        write_lock(fd, 0, SEEK_SET, 0);
        lseek(fd, 0L, SEEK_SET);
        n = read(fd, line, MAXLINE);
        line[n] = 0;

        n = sscanf(line, "%ld\n", &seqno);
        printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long)pid, seqno);
        ++seqno;

        snprintf(line, sizeof(line), "%ld\n", seqno);
        lseek(fd, 0L, SEEK_SET);
        write(fd, line, strlen(line));

        un_lock(fd, 0, SEEK_SET, 0);
    }

    return 0;
}
