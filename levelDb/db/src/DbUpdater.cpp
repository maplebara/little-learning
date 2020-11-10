#include "DbUpdater.h"
#include "DbReqParser.h"
#include "net.h"
#include "DbOperator.h"
#include "leveldb/db.h"
#include <vector>
#include <unistd.h>

extern DbServer server;
extern int notifyFd;

void exec(const Task& task)
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    if(status.ok()) {
        printf("open db succ\n");
    }

    auto* client = server.getClient(task.fd);
    std::vector<LevelDbCommand> cmds;
    int st = lookupCmdTable(task.cmd, cmds);
    if(st < 0) {
        printf("Invalid cmd parameter.\n");
        return ;
    }
    for(auto& command : cmds) {
        if(command.cmd(command.paras, db, client) < 0){
            client->getOutputBuff().append("Error!\n");
            printf("exec cmd fail.\n");
        }
        else {
            if(client)
                client->getOutputBuff().append("OK!\n");
            else {
                printf("fd %d:error\n", task.fd);
            }
        }
    }
}

void handleTask(const Task& task)
{
    auto* client = server.getClient(task.fd);
    if(task.cmd[0] == "exit") {
        printf("exit client!\n");
        client->exit();
    } 
    else {
        exec(task);
    }
    long long fd = notifyFd;
    if(write(notifyFd, &fd, sizeof fd) < 0) {
        perror("notify I/O thread fail");
    }
    else {
        printf("Notify succ\n");
    }
}

void dataUpdate_Entry()
{
    while(true) {
        auto& q = TaskQueues::getInstance();
        Task t;
        printf("q.read before\n");
        q.read(t);
        printf("q.read after\n");
        handleTask(t);
    }
}