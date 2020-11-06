#include "DbOperator.h"


Status db_set(const DbEvent& event, DB* db)
{
    return db->Put(leveldb::WriteOptions(), event.key, event.value);
}

Status db_get(const DbEvent& event, DB* db)
{
    return db->Get(leveldb::ReadOptions(), event.key, &value);
}

Status db_del(const DbEvent& event, DB* db)
{
    return db->Delete(leveldb::WriteOptions(), event.key);
}

namespace {
    using Func = std::function<Status(const DbEvent& event, DB* db)>;

    std::unordered_map<string, Func> funcs = {
        {"set", db_set},
        {"get", db_get},
        {"del", db_del}
    };
}


Status handleEvent(const DbEvent& event)
{
    auto& func = funcs.find(event.cmd);
    if(func == funcs.end()) {
        printf("Not Found the command!");
        return Status::NotFound("Not Found the command!");
    }
    auto stat = func(event, this->db);
    auto* levent = event_new(this->evBase, fd, EV_WRITE, readEventHandler, this->evBase);
    event_add(levent, NULL);
    return stat;
}




