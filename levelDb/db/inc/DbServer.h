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