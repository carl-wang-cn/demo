#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


#define MAXLINE 1024
#define SERV_PORT 9877

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1, stdineof;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
    int n, ret;

    printf("in str_cli\n");
    stdineof = 0;
    FD_ZERO(&rset);
    while (1)
    {
        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;

        assert(-1 != select(maxfdp1, &rset, NULL, NULL, NULL));
        
        if (FD_ISSET(sockfd, &rset))
        {
            if (0 == (n = recv(sockfd, recvline, sizeof(recvline), 0)))
            {
                printf("recv 0\n");
                if (stdineof == 1)
                {
                    return;
                }
                else
                {
                    printf("server terminated prematurely\n");
                    return;
                }
            }
            printf("recv %d\n", n);
            write(stdout, recvline, n);
        }

        if (FD_ISSET(fileno(fp), &rset))
        {
            if ((n = read(fileno(fp), sendline, sizeof(sendline))) == 0)
            {
                printf("read 0\n");
                stdineof = 1;
                shutdown(sockfd, SHUT_WR); //send FIN
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            printf("read %d\n", n);
            send(sockfd, sendline, n, 0);
        }
    }
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    int ret = 0;

    if (argc != 2)
    {
        printf("usage: tcpcli <ipaddress>\n");
        return 0;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd > 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    assert(ret == 0);

    str_cli(stdin, sockfd);

    return 0;
}
