#include "DbServer.h"
#include <event2/event.h>
#include <cassert>
#include <sys/eventfd.h>
#include "Socket.h"
#include <unistd.h>
#include <thread>
#include "DbProxy.h"

const uint16_t listenPort = 12356;
DbServer server;
int notifyFd = -1;

void DbServer::eventLoop()
{
    int sockfd = listenSocket(listenPort);
    auto* evBase = event_base_new();
    assert(evBase);

    auto* listenEvent = event_new(evBase, sockfd, EV_READ|EV_PERSIST, DbServer::acceptEventHandler, evBase);
    if(event_add(listenEvent, NULL) != 0) {
        exit(1);
    }

    notifyFd = eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK);
    assert(notifyFd > 0);
    auto* notifyEvent = event_new(evBase, notifyFd, EV_READ|EV_PERSIST, reply, evBase);
    if(event_add(notifyEvent, NULL) != 0) {
        exit(1);
    }
    std::thread clientReqHandler(dataUpdate_Entry);
    clientReqHandler.detach();
    event_base_loop(evBase, EVLOOP_NO_EXIT_ON_EMPTY); 
}

void DbServer::acceptEventHandler(evutil_socket_t sockfd, short event_type, void* ev)
{
    sockaddr_in cliAddr;
    socklen_t cliLen = sizeof(sockaddr_in);
    int fd = ::accept(sockfd, (sockaddr*)&cliAddr, &cliLen);
    assert(fd > 0);
    setSockNonBlock(fd);
    server.addClientIfNotExited(fd);
    auto* levent = event_new((event_base*)ev, fd, EV_READ|EV_PERSIST, DbServer::processInput, NULL);
    event_add(levent, NULL);
}

void DbServer::processInput(evutil_socket_t fd, short event_type, void* arg)
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

void DbServer::reply(int nfd, short event_type, void* arg)
{
    long long fd = -1;
    read(nfd, &fd, 8);
    auto client = server.getClient((int)fd);
    if(!client) {
        printf("Invalid fd %d.\n", (int)fd);
        close(fd);
        return ;
    }
    if(client->getOutputBuff().empty() && !client->isExit()) {
        printf("No output, the fd is %d\n", (int)fd);
        return ;
    }
    if(client->isExit()) {
        server.freeClient(fd);
        close(fd);
        return ;
    }
    auto& output = client->getOutputBuff();
    {
        std::unique_lock<std::mutex>(client->getLock());
        int st = write((int)fd, output.c_str(), output.size());
        if(st == output.size()) {
            output.clear();
            printf("reply complete.");
        } else if(st < 0 && errno != EAGAIN) {
            server.freeClient(fd);
            close(fd);
        }
        else {
            client->pendReply((event_base*)(arg));
        }
    }
}

void DbServer::dataUpdate_Entry()
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    if(!status.ok()) {
        printf("open db fail\n");
    }
    else {
        DbProxy dbProxy(db);
        while(true) {
            auto& q = TaskQueues::getInstance();
            Task t;
            q.read(t);
            handleTask(t, dbProxy);
        }
    }
}

void DbServer::handleTask(const Task& task, DbProxy& db)
{
    auto client = server.getClient(task.fd);
    if(task.cmd[0] == "exit") {
        client->exit();
    } 
    else {
        if(db.handleCmd(task.cmd, client) == 0) {
            std::unique_lock<std::mutex> mk(client->getLock());
            client->getOutputBuff().append("OK!\n");
        }
        else {
            std::unique_lock<std::mutex> mk(client->getLock());
            client->getOutputBuff().append("Error!\n");
            printf("exec cmd fail.\n");
        }
    }
    long long fd = task.fd;
    if(write(notifyFd, &fd, sizeof fd) < 0) {
        perror("notify I/O thread fail");
    }
}

void DbServer::addClientIfNotExited(int fd) {
    if(!clients.count(fd)) {
        clients.emplace(fd, make_shared<Client>(fd));
    }
}

shared_ptr<Client> DbServer::getClient(int fd) {
    if(clients.count(fd)) {
        return clients[fd];
    }
    else {
        return nullptr;
    }
}

void DbServer::freeClient(int fd) {
    if(!clients.count(fd)) {
        clients.erase(fd);
    }
}



