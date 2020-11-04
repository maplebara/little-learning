#pragma once

#include <event2/event.h>

void eventLoop();

struct AeEvent
{
    event_base* evBase;
};  


struct Task
{
    Task() : data(nullptr) {}
    Task(char* data) : data(data) {}

    char* data;
};





