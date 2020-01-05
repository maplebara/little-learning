#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common.h"

const int backLog = 5;

int acceptSock(uint16_t port)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(sockfd > 0);

    int flag = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)))
    {
        perror("setsockopt");
        exit(1);
    }

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

void ttcpServer(uint16_t port)
{
    int listenFd = acceptSock(port);
    sockaddr_in cliAddr;
    socklen_t cliLen = sizeof(sockaddr_in);

    while(true)
    {
        int fd = accept(listenFd, (sockaddr*)&cliAddr, &cliLen);
        assert(fd >= 0);
        
        SessionMessage sm{0, 0};
        ssize_t smLen = read(fd, &sm, sizeof(sm));
        assert(smLen == sizeof(sm));

        sm.length = ntohl(sm.length);
        sm.number = ntohl(sm.number);

        printf("sessionMsg length = %u\n", sm.length);
        printf("sessionMsg number = %u\n", sm.number);

        uint32_t msgLen = sizeof(PayLoad) + sm.length;
        PayLoad* message = (PayLoad*)malloc(msgLen);
        assert(message != nullptr);

        for(int i = 0; i < sm.number; ++i)
        {
            ssize_t rLen = read(fd, message, msgLen);
            assert(rLen == msgLen);

            uint32_t ack = htonl(message->length);
            ssize_t wLen = write(fd, &ack, sizeof(uint32_t));
            assert(wLen == sizeof(uint32_t));
        }
        free(message);
        close(fd);
    }
    close(listenFd);
}

int main(int argv, char* args[])
{
    ttcpServer(atoi(args[1]));
}





