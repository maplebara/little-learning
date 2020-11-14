#pragma once

#include "leveldb/db.h"
#include "comm_struct.h"

using leveldb::DB;
using std::shared_ptr;
using std::unordered_map;
using std::string;

struct DbProxy
{
    DbProxy(DB* db) : db(db)  {}

    int handleCmd(const vector<string>& cmdPara, shared_ptr<Client>& client);

private:
    int set(const string& key, const string& value);
    int get(const string& key, shared_ptr<Client>& client);
    int del(const string& key);

private:
    DB* db;
};







