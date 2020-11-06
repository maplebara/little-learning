#pragma once 

#include "leveldb/slice.h"

using leveldb::Slice;

enum OpType {
    kAdd,
    kUpdate,
    kDel,
    kBatch
};


struct DbEvent
{
    Slice cmd;
    Slice key;
    Slice value;
};







