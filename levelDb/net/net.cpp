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
    printf("client fd is %d\n", fd);
    auto client = server.getClient(fd);
    if(!client) {
        printf("No valid client, quit!\n");
        close(fd);
        return ;
    }
    if(client->processQuery() == 0) {
        Task task;
        task.fd = fd;
        client->forwardTask(task.cmd);
        for(auto& s : task.cmd) {
            printf("%s ", s.c_str());
        }
        printf("\n");
        TaskQueues::getInstance().write(std::move(task));
    }
    else {
        printf("processQuery fail!\n");
    }
}

void reply(evutil_socket_t nfd, short event_type, void* arg)
{
    long long fd = -1;
    read(nfd, &fd, 8);
    ++fd;
    auto* client = server.getClient((int)fd);
    if(!client) {
        printf("No client, the fd is %d\n", (int)fd);
        return ;
    }
    if(client->isExit()) {
        close(fd);
    } else {
        auto& output = client->getOutputBuff();
        if(!output.empty()) {
            write((int)fd, output.c_str(), output.size());
        }
    }
}

void acceptEventHandler(evutil_socket_t sockfd, short event_type, void *aeEvent)
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
    auto* ev = static_cast<AeEvent*>(aeEvent);
    auto* levent = event_new(ev->evBase, fd, EV_READ|EV_PERSIST, processInput, ev);
    ev->content = levent;
    event_add(levent, NULL);
}

void eventLoop()
{
    int sockfd = listenSocket(listenPort);
    auto* evBase = event_base_new();
    if(!evBase) {
        printf("create event base failed!!\n");
        exit(1);
    }
    AeEvent aeEvent;
    aeEvent.evBase = evBase;

    auto* listenEvent = event_new(evBase, sockfd, EV_READ|EV_PERSIST, acceptEventHandler, (void*)&aeEvent);
    if(event_add(listenEvent, NULL) != 0) {
        exit(1);
    }

    notifyFd = eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK);
    if(notifyFd < 0) {
        perror("Create eventfd fail");
    }
    auto* notifyEvent = event_new(evBase, notifyFd, EV_READ|EV_PERSIST, reply, NULL);
    if(event_add(notifyEvent, NULL) != 0) {
        exit(1);
    }
    std::thread clientReqHandler(dataUpdate_Entry);
    event_base_loop(evBase, EVLOOP_NO_EXIT_ON_EMPTY); 
}




