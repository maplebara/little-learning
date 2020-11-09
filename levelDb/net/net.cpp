#include "net.h"
#include <event2/event.h>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include "ProducerConsumerQueue.h"
#include "TaskQueue.h"
#include <thread>
#include "DbUpdater.h"
#include "Socket.h"
#include "DbServer.h"

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

    }
  //  TaskQueues::getInstance().write(Task(buf, len, fd, ev->evBase));
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

void eventEntry()
{
    server.eventLoop(); 
}




