#pragma once

#include "leveldb/slice.h"
#include "comm_struct.h"
#include <vector>

using leveldb::Slice;

struct DbReqParser
{
    DbReqParser(Slice msg) : msg(msg) {}

    int parse(DbEvent& event);

private:
    int parseSimpleStr(int st, Slice& str);
    int parseBlkStr(int st, Slice& str);
    int parseArray(int st, DbEvent& event);

private:
    Slice msg;
};





