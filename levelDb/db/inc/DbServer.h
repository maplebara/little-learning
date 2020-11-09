#pragma once

#include <unordered_map>
#include <string>
#include <vector>

using std::unordered_map;
using std::string;
using std::vector;

const int QUERY_BUFF_LEN = 14 * 1024;

struct Client
{
    Client() : Client(-1) {}
    Client(int fd) : fd(fd), qryType(0), qry_pos(0), multiBulkCnt(0), bulkLen(-1) {}

    int processQuery();
    
    void forwardTask(vector<string>& rhs) {
        std::swap(rhs, paras);
    }

private:
    int parseQuery();
    int parseBulkQuery();

private:
    int fd;
    int qryType;
    int qry_pos;
    string queryBuff;
    int multiBulkCnt;
    int bulkLen;
    vector<string> paras;
    volatile string output;
};

struct DbServer
{
    void addClientIfNotExited(int fd) {
        if(!clients.count(fd)) {
            clients.emplace(fd, Client(fd));
        }
    }

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