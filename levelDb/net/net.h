#pragma once

#include <event2/event.h>
#include "leveldb/slice.h"

using leveldb::Slice;

void eventLoop();

struct AeEvent
{
    event_base* evBase;
    event* content;
};  


//not thread-safe
struct Task
{
    Task() : data(nullptr), len(0), clientFd(-1), evBase(nullptr), refs(nullptr) {}
    Task(char* data, uint32_t len, int fd, event_base* evBase) : data(data), len(len), clientFd(fd)
                                    , evBase(evBase), refs(new uint32_t(1)) {}

    Task(const Task& rhs) {
        this->data = rhs.data;
        this->len = rhs.len;
        this->clientFd = rhs.clientFd;
        this->evBase = rhs.evBase;
        ++(*rhs.refs);
        this->refs = rhs.refs;
    }

    Task& operator=(const Task& rhs) {
        if(this == &rhs) return *this;
        --(*this->refs);
        if(*this->refs == 0) {
            delete [] data;
            delete refs;
        }
        this->data = rhs.data;
        this->len = rhs.len;
        this->clientFd = rhs.clientFd;
        this->evBase = rhs.evBase;
        ++(*rhs.refs);
        this->refs = rhs.refs;
        return *this;
    }

    ~Task() {
        --(*this->refs);
        if(*this->refs == 0) {
            delete [] data;
            delete refs;
        }
    }

    Slice getMsg() const {
        return Slice(data, len);
    }

    event_base* getEventBase() const {
        return evBase;
    }

    int fd() const {
        return clientFd;
    }

private:
    char* data;
    uint32_t len;
    int clientFd;
    event_base* evBase;
    uint32_t* refs;
};

void acceptEventHandler(evutil_socket_t sockfd, short event_type, void *aeEvent);





