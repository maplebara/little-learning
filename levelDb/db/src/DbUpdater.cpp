#include "DbUpdater.h"
#include "DbReqParser.h"
#include "net.h"
#include "DbOperator.h"
#include "leveldb/db.h"

void handleTask(const Task& task)
{
    DbReqParser reqParser(task.getMsg());
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    DbOperator dbInst(db, task.getEventBase(), task.fd());
    dbInst.handleEvent(reqParser.getEvent());
}

void dataUpdate_Entry()
{
    while(true) {
        auto& q = TaskQueues::getInstance();
        Task t;
        q.read(t);
        handleTask(t);
    }
}