#include "DbProxy.h"
#include <unistd.h>
#include <string.h>
#include <functional>
#include <unordered_map>
#include <string>

int DbProxy::set(const string& key, const string& value)
{
    printf("set key[%s], value[%s]\n", key.c_str(), value.c_str());
    auto res = db->Put(leveldb::WriteOptions(), key, value);
    return res.ok() ? 0 : -1;
}

int DbProxy::get(const string& key, shared_ptr<Client>& client)
{
    auto res = db->Get(leveldb::ReadOptions(), key, &client->getOutputBuff());
    if(res.ok()) {
        client->getOutputBuff().append("\n");
        return 0;
    }
    return -1;
}

int DbProxy::del(const string& key)
{
    auto res = db->Delete(leveldb::WriteOptions(), key);
    return res.ok() ? 0 : -1;
}

int DbProxy::handleCmd(const vector<string>& cmdPara, shared_ptr<Client>& client)
{
    int st = 0, stat = 0;
    while(st < cmdPara.size()) {
        if(cmdPara[st] == "set" && st + 2 < cmdPara.size()) {
            stat = set(cmdPara[st + 1], cmdPara[st + 2]);
            st += 2;
        }
        else if(cmdPara[st] == "get" && st + 1 < cmdPara.size()) {
            std::unique_lock<std::mutex> mk(client->getLock());
            stat = get(cmdPara[++st], client);
        }
        else if(cmdPara[st] == "del") {
            stat = del(cmdPara[++st]);
        }
        else {
            printf("Invalid command!\n");
            return -1;
        }
        if(stat < 0) return stat;
        ++st;
    }
    return 0;
}




