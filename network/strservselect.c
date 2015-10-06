#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


#define MAXLINE 1024
#define SERV_PORT 9877
#define LISTENQ 1024

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

int main(int argc, char **argv)
{
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    int ret;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd > 0);

    int32_t reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    assert(ret != -1);

    ret = listen(listenfd, LISTENQ);
    assert(ret != -1);

    maxfd = listenfd;
    maxi = -1;
    for (i=0; i<FD_SETSIZE; i++)
    {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while (1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) //new client connection
        {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
            assert(connfd != -1);
            printf("accept connfd:%d\n", connfd);

            for (i=0; i < FD_SETSIZE; i++)
            {
                if (client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }
            if (i == FD_SETSIZE)
            {
                printf("too many clients\n");
                exit(0);
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd)
            {
                maxfd = connfd;
            }

            if (i > maxi)
            {
                maxi = i;       //max index in client array
            }

            if (--nready <= 0)
            {
                continue; //no more readable descriptors
            }
        }

        while (1)   //check all clients for data
        {
            if ((sockfd = client[i]) < 0)
            {
                continue;
            }

            if (FD_ISSET(sockfd, &rset))
            {
                if ((n = recv(sockfd, buf, sizeof(buf), 0)) == 0)
                {
                    //connection closed by client
                    printf("recv 0\n");
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else
                {
                    printf("recv %d\n", n);
                    send(sockfd, buf, n, 0);
                }

                if (--nready <= 0)
                {
                    printf("no more readable descriptors\n");
                    break;  //no more readable descriptors
                }
            }
        }
    }

    return 0;
}
