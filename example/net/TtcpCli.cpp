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

#include "common.h"

void ttcpClient(const char* hostName, uint16_t port, uint32_t msgLen, uint32_t msgNum)
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

    SessionMessage sm;
    sm.length = htonl(msgLen);
    sm.number = htonl(msgNum);

    uint32_t len = sizeof(PayLoad) + msgLen;
    PayLoad* message = (PayLoad*)malloc(len);
    assert(message);
    message->length = htonl(msgLen);
    for(int i = 0; i < msgLen; ++i)
    {
        message->payLoad[i] = "0123456789ABCDEF"[i % 16];
    }

    for(int i = 0; i < msgNum; ++i)
    {
        ssize_t wlen = write(sockfd, message, len);
        assert(wlen == len);

        uint32_t ack = 0;
        ssize_t rLen = read(sockfd, &ack, sizeof(uint32_t));
        assert(rLen == sizeof(uint32_t));
    }

    free(message);
    close(sockfd);
}

int main(int argv, char* args[])
{
    ttcpClient(args[1], atoi(args[2]), atoi(args[3]), atoi(args[4]));
}
