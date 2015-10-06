#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

static int create_and_bind(char *port)
{
    struct addrinfo hints = {};
    struct addrinfo *result = NULL;
    struct addrinfo *rp = NULL;
    int s = 0;
    int sfd = 0;

    hints.ai_family = AF_UNSPEC;  //Return IPv4 and IPv6 choices
    hints.ai_socktype = SOCK_STREAM;  // TCP socket
    hints.ai_flags = AI_PASSIVE;  //All interfaces

    s = getaddrinfo(NULL, port, &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
        {
            continue;
        }
        s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0)
        {
            // we managed to bind successfully
            break;
        }
        close(sfd);
    }

    if (rp == NULL)
    {
        fprintf(stderr, "could not bind\n");
        return -1;
    }

    freeaddrinfo(result);

    return sfd;
}

static int make_socket_non_blocking(int sfd)
{
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror("fcntl");
        return -1;
    }

    return 0;
}

#define MAXEVENTS 64
int main(int argc, char **argv)
{
    int sfd = 0, s = 0;
    int efd = 0;
    struct epoll_event event = {};
    struct epoll_event *events = NULL;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sfd = create_and_bind(argv[1]);
    if (sfd == -1) {abort();}
    fprintf(stdout, "sfd: %d\n", sfd);

    s = make_socket_non_blocking(sfd);
    if (s == -1) {abort();}

    s = listen(sfd, SOMAXCONN);
    if (s == -1)
    {
        perror("listen");
        abort();
    }

    efd = epoll_create(256);
    if (efd == -1)
    {
        perror("epoll_create1");
        abort();
    }

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1)
    {
        perror("epoll_ctl");
        abort();
    }

    //Buffer where events are returned
    events = calloc(MAXEVENTS, sizeof(event));

    fprintf(stdout, "before event loop\n");
    //The event loop
    while (1)
    {
        int n, i;

        n = epoll_wait(efd, events, MAXEVENTS, -1);
        fprintf(stdout, "epoll wait return: %d\n", n);
        for (i=0; i<n; i++)
        {
            fprintf(stdout, "events[%d].data.fd=%d\n", i, events[i].data.fd);
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                    || (!(events[i].events & EPOLLIN)))
            {
                //an error has occured on this fd, or the socket is not ready for reading
                fprintf(stderr, "epoll error\n");
                close(events[i].data.fd);
                continue;
            }
            else if (sfd == events[i].data.fd)
            {
                //we have a notification on the listening socket, which means one or more
                //incoming connections
                fprintf(stdout, "sfd == events[i].data.fd\n");
                while (1)
                {
                    struct sockaddr in_addr = {};
                    socklen_t in_len = 0;
                    int infd = 0;
                    char hbuf[NI_MAXHOST] = {}, sbuf[NI_MAXSERV] = {};

                    in_len = sizeof(in_addr);
                    infd = accept(sfd, &in_addr, &in_len);
                    if (infd == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            //we have processed all incoming connections
                            fprintf(stderr, "accept error: %d\n", errno);
                            break;
                        }
                        else
                        {
                            perror("accept");
                            break;
                        }
                    }
                    fprintf(stdout, "infd=%d\n", infd);

                    s = getnameinfo(&in_addr, in_len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), 
                            NI_NUMERICHOST | NI_NUMERICSERV);
                    if (s == 0)
                    {
                        printf("accepted connection on descriptor %d (host=%s, port=%s)\n", 
                                infd, hbuf, sbuf);
                    }

                    fprintf(stdout, "getnameinfo return s:%d\n", s);
                    //make the incoming socket non-blocking and add it to the list of fds
                    //to monitor
                    s = make_socket_non_blocking(infd);
                    if (s == -1) {abort();}

                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1)
                    {
                        perror("epoll_ctl");
                        abort();
                    }
                }
                continue;
            }
            else
            {
                //we have data on the fd waiting to be read. Read and display it. We must read
                //whatever data is available completely, as we are running in edge-triggered 
                //mode and won't get a notification again for the same data
                int done = 0;
                
                fprintf(stdout, "else\n");
                while (1)
                {
                    ssize_t count = 0;
                    char buf[512] = "hello epoll";

                    count  = read(events[i].data.fd, buf, sizeof(buf));
                    fprintf(stdout, "read return count=%d\n", count);
                    if (count == -1)
                    {
                        //if errno == EAGAIN , that means we have read all data.
                        //So go back to the main loop
                        fprintf(stdout, "count==-1, errno:%d\n", errno);
                        if (errno != EAGAIN)
                        {
                            perror("read");
                            done = 1;
                        }
                        break;
                    }
                    else if (count == 0)
                    {
                        //end of file. The remote has closed the connection
                        fprintf(stdout, "count == 0\n");
                        done = -1;
                        break;
                    }

                    printf("read data: %s\n", buf);
                    //write the buffer to standard output
                    s = write(STDOUT_FILENO, buf, count);
                    if (s == -1)
                    {
                        perror("write");
                        abort();
                    }
                }

                if (done)
                {
                    printf("closed connection on descriptor %d\n", events[i].data.fd);

                    //closing the descriptor will make epoll remove it from the set of
                    //descriptors which are monitored
                    close(events[i].data.fd);
                }
            }
        }
    }

    free(events);
    events = NULL;
    close(sfd);

    return EXIT_SUCCESS;
}
