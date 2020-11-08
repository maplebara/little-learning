#include "net.h"
#include <event2/event.h>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include "ProducerConsumerQueue.h"
#include "TaskQueue.h"
#include <thread>
#include "DbUpdater.h"

const int backLog = 20;
const uint16_t listenPort = 12357;

int acceptSock(uint16_t port)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, IPPROTO_TCP);
    assert(sockfd > 0);

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

void readEventHandler(evutil_socket_t fd, short event_type, void* arg)
{
    printf("the connection fd is %d\n", fd);
    char* buf = new char[MAX_BUF];
    ssize_t len = ::read(fd, buf, sizeof buf);
    if(len < 0) {
        delete [] buf;
        close(fd);
        perror("read ocurr error!!");
        return ;
    }
    printf("%s, %zd\n", buf, len);
    const char* eStr = "exit";
    if(memcmp(buf, eStr, strlen(eStr)) == 0) {
        auto* ev = static_cast<AeEvent*>(arg);
        event_del(ev->content);
        event_free(ev->content);
        close(fd);
    }
    TaskQueues::getInstance().write(Task(buf, len, fd, (event_base*)arg));
}

void listenEventHandler(evutil_socket_t sockfd, short event_type, void *aeEvent)
{
    sockaddr_in cliAddr;
    socklen_t cliLen = sizeof(sockaddr_in);
    int fd = ::accept(sockfd, (sockaddr*)&cliAddr, &cliLen);
    auto* ev = static_cast<AeEvent*>(aeEvent);
    auto* levent = event_new(ev->evBase, fd, EV_READ|EV_PERSIST, readEventHandler, ev);
    ev->content = levent;
    event_add(levent, NULL);
}

void eventLoop()
{
    int sockfd = acceptSock(listenPort);
    auto* evBase = event_base_new();
    if(!evBase) {
        printf("create event base failed!!\n");
        return ;
    }
    AeEvent aeEvent;
    aeEvent.evBase = evBase;

    auto* listen_event = event_new(evBase, sockfd, EV_READ|EV_PERSIST, listenEventHandler, (void*)&aeEvent);
    int stat = event_add(listen_event, NULL);
    if(stat) printf("stat[%d]\n", stat);

    std::thread clientReqHandler(dataUpdate_Entry);
    event_base_loop(evBase, EVLOOP_NO_EXIT_ON_EMPTY);   
}




