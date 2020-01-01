#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const int backLog = 5;

int acceptSock(uint16_t port)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(sockfd > 0);

    sockaddr_in serverAddr;
    memset(&serverAddr, 0x0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int status = bind(sockfd, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
    assert(status == 0);

    status = listen(sockfd, backLog);
    assert(status == 0);

    return sockfd;
}

void echoServer(uint16_t port)
{
    int sockfd = acceptSock(port);
    sockaddr_in cliAddr;
    socklen_t cliLen = sizeof(sockaddr_in);
    char buff[1024] = {0};

    while(true)
    {
        int fd = accept(sockfd, (sockaddr*)&cliAddr, &cliLen);
        assert(fd >= 0);
        ssize_t len = 0;
        do {
            len = read(fd, buff, sizeof(buff));
            write(fd, buff, len);
        } while(len > 0);
        close(fd);
    }
}

/*
int main(int argv, char* args[])
{
    uint16_t port = atoi(args[1]);
    echoServer(port);

    printf("the server end!!\n");

    return 0;
}
*/
