#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int sockfd = -1;
    int len = 0;
    struct sockaddr_in address = {};
    int result = 0;
    char ch = 'A'; //data to send
    char recv[5] = {};
    int recv_len = 0;

    //创建流套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //设置要连接的服务器信息
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8002);

    len = sizeof(address);

    //连接到服务器
    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("ops:client\n");
        exit(1);
    }

    //发送请求给服务器
    write(sockfd, &ch, 1);

    //从服务器获取数据
    while (1)
    {
        recv_len = read(sockfd, recv, 4);
        if (recv_len <= 0)
            break;
        printf("data from server = %s \n", recv);
    }
    close(sockfd);

    return 0;
}
