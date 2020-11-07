#include "DbOperator.h"
#include <unistd.h>
#include <string.h>
#include "comm_strcut.h"
#include "DbStatus.h"

namespace {
    DbResponse DB_SUCC_RSP;
    DbResponse DB_FAIL_RSP(Status::Corruption("Normal Error!"));
}

DbResponse* db_set(const DbEvent& event, DB* db)
{
    auto res = db->Put(leveldb::WriteOptions(), event.key, event.value);
    return res.ok() ? &DB_SUCC_RSP : &DB_FAIL_RSP;
}

DbResponse* db_get(const DbEvent& event, DB* db)
{
    GetDbResponse* response = new GetDbResponse();
    auto res = db->Get(leveldb::ReadOptions(), event.key, response->getRsp());
    response->set(res);
    return response;
}

DbResponse* db_del(const DbEvent& event, DB* db)
{
    auto res = db->Delete(leveldb::WriteOptions(), event.key);
    return res.ok() ? &DB_SUCC_RSP : &DB_FAIL_RSP;
}

void handleRsp(evutil_socket_t fd, short event_type, void* rsp);
{
    if(!rsp) {
        fprintf(stderr, "No db response!\n");
        return ;
    }
    auto* dbResponse = static_cast<AbstractDbResponse*>(rsp);
    int st = dbResponse->response(fd);
    if(st < 0) {
        printf("handleRsp fail!\n");
    }
    response->destroy();
}

namespace {
    using Func = std::function<AbstractDbResponse* (const DbEvent& event, DB* db)>;

    std::unordered_map<string, Func> funcs = {
        {"set", db_set},
        {"get", db_get},
        {"del", db_del}
    };
}


void DbOperator::handleEvent(const DbEvent& event)
{
    auto& func = funcs.find(event.cmd);
    if(func == funcs.end()) {
        printf("Not Found the command!");
        return ;
    }
    auto* rsp = func(event, this->db);
    auto* levent = event_new(this->evBase, this->clientFd, EV_WRITE, handleRsp, rsp);
    event_add(levent, NULL);
}




