#include "DbUpdater.h"
#include "net.h"
#include "DbOperator.h"
#include "leveldb/db.h"
#include <vector>
#include <unistd.h>

extern DbServer server;
extern int notifyFd;

int exec(const Task& task, leveldb::DB* db)
{
    std::vector<LevelDbCommand> cmds;
    if(lookupCmdTable(task.cmd, cmds) < 0) {
        printf("Invalid cmd parameter.\n");
        return -1;
    }
    auto client = server.getClient(task.fd);
    if(!client) {
        printf("No the client!\n");
        return -1;
    }

    for(auto& command : cmds) {
        if(command.cmd(command.paras, db, client) < 0){
            client->getOutputBuff().append("Error!\n");
            printf("exec cmd fail.\n");
        }
        else {
            client->getOutputBuff().append("OK!\n");
        }
    }
    return 0;
}

void handleTask(const Task& task, leveldb::DB* db)
{
    auto client = server.getClient(task.fd);
    if(task.cmd[0] == "exit") {
        client->exit();
    } 
    else {
        if(exec(task, db) < 0) {
            printf("exec command fail.\n");
            return ;
        }
    }
    long long fd = notifyFd;
    if(write(notifyFd, &fd, sizeof fd) < 0) {
        perror("notify I/O thread fail");
    }
}

void dataUpdate_Entry()
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    if(!status.ok()) {
        printf("open db fail\n");
    }
    else {
        while(true) {
            auto& q = TaskQueues::getInstance();
            Task t;
            q.read(t);
            handleTask(t, db);
        }
    }
}