#include "DbOperator.h"
#include <unistd.h>
#include <string.h>
#include <functional>
#include <unordered_map>
#include <string>

int db_set(const vector<const string*>& paras, DB* db, std::shared_ptr<Client>& client)
{
    printf("set key[%s], value[%s]\n", paras[0]->c_str(), paras[1]->c_str());
    auto res = db->Put(leveldb::WriteOptions(), *paras[0], *paras[1]);
    return res.ok() ? 0 : -1;
}

int db_get(const vector<const string*>& paras, DB* db, std::shared_ptr<Client>& client)
{
    
    auto res = db->Get(leveldb::ReadOptions(), *paras[0], &client->getOutputBuff());
    if(res.ok()) {
        client->getOutputBuff().append("\n");
        return 0;
    }
    return -1;
}

int db_del(const vector<const string*>& paras, DB* db, std::shared_ptr<Client>& client)
{
    auto res = db->Delete(leveldb::WriteOptions(), *paras[0]);
    return res.ok() ? 0 : -1;
}

namespace {
    std::unordered_map<std::string, LevelDbCommand> cmdTable = {
        {"set", {"set", 2, db_set}},
        {"get", {"get", 1, db_get}},
        {"del", {"del", 1, db_del}}
    };
}

int lookupCmdTable(const vector<string>& cmdPara, vector<LevelDbCommand>& cmds)
{
    int st = 0;
    while(st < cmdPara.size()) {
        if(!cmdTable.count(cmdPara[st]))
            return -1;
        auto cmd = cmdTable[cmdPara[st]];
        for(int i = 0; i < cmd.paraNum && (st + 1) < cmdPara.size(); ++i) {
            cmd.paras.push_back(&cmdPara[++st]);
        }
        ++st;
        cmds.push_back(std::move(cmd));
    }
    return 0;
}




