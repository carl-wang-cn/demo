#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
    int server_sockfd = -1;
    int server_len = 0;
    int client_len = 0;
    char buffer[512] = {};
    int result = 0;
    struct sockaddr_in server_addr = {};
    struct sockaddr_in client_addr = {};

    //创建数据报套接字
    server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //设置监听IP端口
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9739);
    server_len = sizeof(server_addr);

    //绑定套接字
    bind(server_sockfd, (struct sockaddr *)&server_addr, server_len);

    //忽略子进程停止或推出信号
    signal(SIGCHLD, SIG_IGN);

    while (1)
    {
        //接收数据，用client_addr来储存数据来源程序的IP端口
        result = recvfrom(server_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);

        if (fork() == 0)
        {
            //利用子进程来处理数据
            buffer[0] += 'a' - 'A';
            sleep(5);

            //发送处理后的数据
            sendto(server_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, client_len);
            printf("%c\n", buffer[0]);
            //注意一定要关闭子进程，要不然程序运行会异常
            exit(0);
        }
    }

    close(server_sockfd);
}
