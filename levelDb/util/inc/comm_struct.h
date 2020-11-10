#pragma once 

#include "leveldb/slice.h"
#include <vector>
#include <string>
#include "leveldb/db.h"
#include "DbServer.h"

using leveldb::Slice;
using leveldb::DB;
using std::vector;
using std::string;

struct DbEvent
{
    Slice cmd[3];
};


struct Task
{
    int fd;
    vector<string> cmd;
};

typedef int (*cmdCallback)(const vector<const string*>&, DB*, Client*);

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






