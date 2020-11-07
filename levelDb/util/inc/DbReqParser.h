#pragma once

#include "leveldb/slice.h"
#include "comm_strcut.h"
#include <vector>

using leveldb::Slice;

struct CmdStr
{
    OpType op;
    Slice key;
    Slice value;
};

struct DbReqParser
{
    DbReqParser(Slice msg) : msg(msg) {}

    DbEvent getEvent();

private:
    int parse();
    int parseString();
    int parseArray();

private:
    Slice msg;
    std::vector<CmdStr> cmds;
};





