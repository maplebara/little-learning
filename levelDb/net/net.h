#pragma once

#include <event2/event.h>

void eventLoop();

struct AeEvent
{
    event_base* evBase;
};  


//not thread-safe
struct Task
{
    Task() : data(nullptr), len(0), evBase(nullptr), refs(nullptr) {}
    Task(char* data, uint32_t len, event_base* evBase) : data(data), len(len)
                                    , evBase(evBase), refs(new uint32_t(1)) {}

    Task(const Task& rhs) {
        this->data = rhs.data;
        this->len = rhs.len;
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

    const char* getMsg() const {
        return data;
    }

    event_base* getEventBase() {
        return evBase;
    }

private:
    char* data;
    uint32_t len;
    event_base* evBase;
    uint32_t* refs;
};





