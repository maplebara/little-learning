#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>
#include <string>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

void echoClient(const char* hostName, uint16_t port)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(sockfd > 0);

    hostent* host = gethostbyname(hostName);

    sockaddr_in serverAddr;
    memset(&serverAddr, 0x0, sizeof(sockaddr_in));
    serverAddr.sin_family = host->h_addrtype;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr = *(in_addr*)host->h_addr;
    assert(host->h_addrtype == AF_INET && host->h_length == sizeof(uint32_t));
    int status = connect(sockfd, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
    assert(status == 0);

    char buff[1024];
    char data[1024];
    while(std::cin.getline(buff, sizeof(buff)))
    {
        write(sockfd, buff, strlen(buff));

        if(read(sockfd, data, sizeof(data)) == 0)
            perror("the string end!!\n");

        printf("normal reflect: %s\n", data);
        memset(data, 0, sizeof(data));
    }
    close(sockfd);
}

/**
int main(int argv, char* args[])
{
    uint16_t port = atoi(args[2]);
    echoClient(args[1], port);

    printf("the client end!!\n");

    return 0;
}
*/
