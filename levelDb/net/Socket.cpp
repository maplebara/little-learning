#include "Socket.h"
#include <sys/types.h>        
#include <sys/socket.h>
#include <cassert>
#include <cstring>
#include <event2/event.h>

namespace {
    const int backLog = 20;
}

int setSockNonBlock(int fd)
{
    int flags;

    /* Set the socket blocking (if non_block is zero) or non-blocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        perror("fcntl(F_GETFL)");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl(F_SETFL,O_NONBLOCK)");
        return -1;
    }
    return 0;
}

int listenSocket(uint16_t port)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, IPPROTO_TCP);
    assert(sockfd > 0);
    int val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &val, sizeof val);
    sockaddr_in serverAddr;
    memset(&serverAddr, 0x00, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int status = bind(sockfd, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
    assert(status == 0);

    status = listen(sockfd, backLog);
    assert(status == 0);

    return sockfd;
}


