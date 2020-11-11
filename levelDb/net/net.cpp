#include "net.h"
#include <event2/event.h>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <thread>
#include "ProducerConsumerQueue.h"
#include "TaskQueue.h"
#include "DbUpdater.h"
#include "Socket.h"
#include "DbServer.h"
#include "comm_struct.h"
#include <sys/eventfd.h>

const uint16_t listenPort = 12357;
DbServer server;
int notifyFd = -1;

void processInput(evutil_socket_t fd, short event_type, void* arg)
{
    auto client = server.getClient(fd);
    if(!client) {
        printf("No valid client fd[%d], quit!\n", fd);
        close(fd);
        return ;
    }
    if(client->processQuery() == 0) {
        Task task;
        task.fd = fd;
        client->forwardTask(task.cmd);
        TaskQueues::getInstance().write(std::move(task));
    }
    else if(client->isExit()) {
        server.freeClient(fd);
        close(fd);
    }
}

void acceptEventHandler(evutil_socket_t sockfd, short event_type, void* ev)
{
    sockaddr_in cliAddr;
    socklen_t cliLen = sizeof(sockaddr_in);
    int fd = ::accept(sockfd, (sockaddr*)&cliAddr, &cliLen);
    if(fd < 0) {
        perror("accept fail");
        return ;
    }
    setSockNonBlock(fd);
    server.addClientIfNotExited(fd);
    auto* levent = event_new((event_base*)ev, fd, EV_READ|EV_PERSIST, processInput, NULL);
    event_add(levent, NULL);
}

void eventLoop()
{
    int sockfd = listenSocket(listenPort);
    auto* evBase = event_base_new();
    if(!evBase) {
        exit(1);
    }

    auto* listenEvent = event_new(evBase, sockfd, EV_READ|EV_PERSIST, acceptEventHandler, evBase);
    if(event_add(listenEvent, NULL) != 0) {
        exit(1);
    }

    notifyFd = eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK);
    if(notifyFd < 0) {
        perror("Create eventfd fail");
        exit(1);
    }
    auto* notifyEvent = event_new(evBase, notifyFd, EV_READ|EV_PERSIST, Client::reply, evBase);
    if(event_add(notifyEvent, NULL) != 0) {
        exit(1);
    }
    std::thread clientReqHandler(dataUpdate_Entry);
    clientReqHandler.detach();
    event_base_loop(evBase, EVLOOP_NO_EXIT_ON_EMPTY); 
}




