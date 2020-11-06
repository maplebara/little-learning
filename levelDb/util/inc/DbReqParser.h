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
    DbReqParser(const char* data) : data(data) {}

    Slice key();
    Slice value();
    OpType ops();

private:
    int parse();
    int parseString();
    int parseArray();

private:
    const char* data;
    std::vector<CmdStr> cmds;
};





