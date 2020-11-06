#pragma once

#include "leveldb/slice.h"
#include "leveldb/db.h"
#include "comm_struct.h"
#include <event2/event.h>

using leveldb::Slice;
using leveldb::DB;
using leveldb::Status;

struct DbOperator
{
    DbSetter(DB* db, event_base* evBase) : db(db)，evBase(evBase)  {}

    Status handleEvent(const DbEvent& event);
    Status set(Slice key, Slice value);
    Status get(Slice key);

private:
    DB* db;
    event_base* evBase;
};







