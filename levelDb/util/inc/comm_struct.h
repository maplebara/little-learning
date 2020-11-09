#pragma once 

#include "leveldb/slice.h"
#include <vector>
#include <string>

using leveldb::Slice;
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






