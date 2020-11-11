#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

using std::unordered_map;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

const int QUERY_BUFF_LEN = 14 * 1024;

struct Client
{
    Client() : Client(-1) {}
    Client(int fd) : isExited(false), fd(fd), qryType(0), 
                        qry_pos(0), multiBulkCnt(0), bulkLen(-1) {}

    int processQuery();
    
    void forwardTask(vector<string>& rhs) {
        std::swap(rhs, paras);
    }

    string& getOutputBuff() {
        return output;
    }

    void exit() {
        isExited = true;
    }

    bool isExit() {
        return isExited;
    }

    void pendReply(event_base* evBase, int fd)
    {
        if(!replyEvent) {
            replyEvent = event_new(evBase, fd, EV_WRITE, reply, evBase);
        }
        event_add(replyEvent, NULL);
    }

    static void reply(int nfd, short event_type, void* arg);
    static void reReply(int fd, event_base* evBase);

    ~Client() {
        if(replyEvent) {
            event_del(replyEvent);  
            event_free(replyEvent);    
        }
    }

private:
    int parseQuery();
    int parseBulkQuery();

private:
    bool isExited;
    int fd;
    int qryType;
    int qry_pos;
    string queryBuff;
    int multiBulkCnt;
    int bulkLen;
    vector<string> paras;
    string output;
    event* replyEvent{nullptr};
};

struct DbServer
{
    void addClientIfNotExited(int fd) {
        if(!clients.count(fd)) {
            clients.emplace(fd, make_shared<Client>(fd));
        }
    }

    shared_ptr<Client> getClient(int fd) {
        if(clients.count(fd)) {
            return clients[fd];
        }
        else {
            return nullptr;
        }
    }

    void freeClient(int fd) {
        if(!clients.count(fd)) {
            clients.erase(fd);
        }
    }

private:
    unordered_map<int, shared_ptr<Client>> clients;
};