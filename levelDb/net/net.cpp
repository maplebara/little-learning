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
const int QUERY_BUFF_LEN = 14 * 1024;
DbServer server;

void processInput(evutil_socket_t fd, short event_type, void* arg)
{
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
        TaskQueues::getInstance().write(std::move(task));
    }
}

void reply(evutil_socket_t fd, short event_type, void* arg)
{

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

    int ev_fd = eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK);
    if(ev_fd < 0) {
        perror("Create eventfd fail");
    }
    auto* notifyEvent = event_new(evBase, ev_fd, EV_READ|EV_PERSIST, reply, NULL);
    if(event_add(notifyEvent, NULL) != 0) {
        exit(1);
    }
    std::thread clientReqHandler(dataUpdate_Entry, ev_fd);
    event_base_loop(evBase, EVLOOP_NO_EXIT_ON_EMPTY); 
}




