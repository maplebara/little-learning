#pragma once 

#include "leveldb/slice.h"
#include <vector>
#include <string>
#include "leveldb/db.h"
#include <memory>
#include "DbClient.h"

using leveldb::Slice;
using leveldb::DB;
using std::vector;
using std::string;

struct Task
{
    int fd;
    vector<string> cmd;
};

typedef int (*cmdCallback)(const vector<const string*>&, DB*, std::shared_ptr<Client>&);

struct LevelDbCommand
{
    LevelDbCommand(){}
    LevelDbCommand(const char* name, int num, cmdCallback cmd)
            : name(name), paraNum(num), cmd(cmd) {}

    const char* name;
    int paraNum;
    vector<const string*> paras;
    cmdCallback cmd;
};






