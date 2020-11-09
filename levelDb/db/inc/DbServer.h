#pragma once

#include <unordered_map>
#include <string>
#include <vector>
// #include <event2/event.h>

using std::unordered_map;
using std::string;
using std::vector;

const int QUERY_BUFF_LEN = 14 * 1024;

struct Client
{
    Client() : Client(-1) {}
    Client(int fd) : fd(fd), qryType(0), qry_pos(0), queryBuff(QUERY_BUFF_LEN)
                    , multiBulkCnt(0), bulkLen(-1), paraNum(0) {}

    int processQuery();

private:
    int fd;
    int qryType;
    int qry_pos;
    string queryBuff;
    int multiBulkCnt;
    int bulkLen;
    int paraNum;
    vector<string> paras;
};

struct DbServer
{
    void addClientIfNotExited(int fd) {
        if(!clients.count(fd)) {
            clients[fd] = Client(fd);
        }
    }

    void eventLoop();

    Client* getClient(int fd) {
        if(clients.count(fd)) {
            return &clients[fd];
        }
        else {
            return nullptr;
        }
    }

private:
    unordered_map<int, Client> clients;
};