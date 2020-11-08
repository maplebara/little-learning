#pragma once 

#include "leveldb/slice.h"

using leveldb::Slice;

struct DbEvent
{
    Slice cmd[3];
};







