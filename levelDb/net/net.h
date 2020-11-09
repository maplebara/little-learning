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

void acceptEventHandler(evutil_socket_t sockfd, short event_type, void *aeEvent);





