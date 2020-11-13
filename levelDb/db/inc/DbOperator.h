#pragma once

#include "leveldb/slice.h"
#include "leveldb/db.h"
#include "comm_struct.h"
#include <event2/event.h>

using leveldb::Slice;
using leveldb::DB;
using leveldb::Status;

// struct DbOperator
// {
//     DbOperator(DB* db, event_base* evBase, int fd) : db(db), evBase(evBase), clientFd(fd)  {}

//     void handleEvent(const DbEvent& event);

// private:
//     DB* db;
//     event_base* evBase;
//     int clientFd;
// };

int lookupCmdTable(const vector<string>& cmdPara, vector<LevelDbCommand>& cmds);







