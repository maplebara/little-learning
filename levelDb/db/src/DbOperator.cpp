#include "DbOperator.h"
#include <unistd.h>
#include <string.h>
#include "DbStatus.h"
#include <functional>
#include <unordered_map>
#include <string>

namespace {
    DbResponse DB_SUCC_RSP;
    DbResponse DB_FAIL_RSP(Status::Corruption("Normal Error!"));
}

AbstractDbResponse* db_set(const DbEvent& event, DB* db)
{
    printf("set key[%s], value[%s]\n", event.cmd[1].ToString().c_str(), event.cmd[2].ToString().c_str());
    auto res = db->Put(leveldb::WriteOptions(), event.cmd[1], event.cmd[2]);
    return res.ok() ? &DB_SUCC_RSP : &DB_FAIL_RSP;
}

AbstractDbResponse* db_get(const DbEvent& event, DB* db)
{
    GetDbResponse* response = new GetDbResponse();
    auto res = db->Get(leveldb::ReadOptions(), event.cmd[1], response->getRsp());
    response->set(res);
    return response;
}

AbstractDbResponse* db_del(const DbEvent& event, DB* db)
{
    auto res = db->Delete(leveldb::WriteOptions(), event.cmd[1]);
    return res.ok() ? &DB_SUCC_RSP : &DB_FAIL_RSP;
}

void handleRsp(evutil_socket_t fd, short event_type, void* rsp)
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
    if(dbResponse->id() == kGetDbResponse) {
        delete dbResponse;
    }
}

namespace {
    using Func = std::function<AbstractDbResponse* (const DbEvent& event, DB* db)>;

    std::unordered_map<std::string, Func> funcs = {
        {"set", db_set},
        {"get", db_get},
        {"del", db_del}
    };
}


void DbOperator::handleEvent(const DbEvent& event)
{
    auto func = funcs.find(event.cmd[0].ToString());
    if(func == funcs.end()) {
        printf("Not Found the command!");
        return ;
    }
    auto* rsp = func->second(event, this->db);
    auto* levent = event_new(this->evBase, this->clientFd, EV_WRITE, handleRsp, rsp);
    event_add(levent, NULL);
}




