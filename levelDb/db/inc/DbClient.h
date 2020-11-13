#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <event2/event.h>
#include <mutex>

using std::string;
using std::vector;

const int QUERY_BUFF_LEN = 14 * 1024;

void reReply(int fd, short event_type, void* arg);

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

    void pendReply(event_base* evBase)
    {
        if(!replyEvent) {
            replyEvent = event_new(evBase, fd, EV_WRITE, reReply, evBase);
        }
        event_add(replyEvent, NULL);
    }

    std::mutex& getLock() {
        return op_mutex;
    }

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
    std::mutex op_mutex;
};