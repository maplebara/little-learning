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
    Task() : data(nullptr), len(0), refs(nullptr) {}
    Task(char* data, uint32_t len) : data(data), len(len), refs(new uint32_t(1)) {}

    Task(const Task& rhs) {
        this->data = rhs.data;
        this->len = rhs.len;
        ++(*rhs.refs);
        this->refs = rhs.refs;
    }

    Task& operator=(const Task& rhs) {
        if(this == &rhs) return *this;
        --(*this->refs);
        if(*this->refs == 0) {
            delete [] data;
        }
        this->data = rhs.data;
        this->len = rhs.len;
        ++(*rhs.refs);
        this->refs = rhs.refs;
        return *this;
    }

    ~Task() {
        --(*this->refs);
        if(*this->refs == 0) {
            delete [] data;
        }
    }

private:
    char* data;
    uint32_t len;
    uint32_t* refs;
};





