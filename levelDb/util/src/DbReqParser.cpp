#include "DbReqParser.h"
#include <unordered_map>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace {
    std::unordered_map<std::string, OpType> = { {"open", kAdd}, {"set", kDel}, {"get", kUpdate} };
}

int splite(vector<Slice>& strVec, Slice str)
{
    int i = 0, j;
    while(i < str.size()) {
        j = i;
        while(j < str.size() && str[j] != '\r') ++j;
        if(str[j] != '\r') return -1;
        strVec.push_back(Slice(str.data() + i, j - i));
        i = j + 2;
    }
    return 0;
}

int DbReqParser::parse(int st, DbEvent& event, const vector<Slice>& strVec)
{
    if(strVec[st][0] == '*') {
        int cnt = 0;
        for(int i = 1; i < strVec[st].size(); ++i)
            cnt = 10 * cnt + strVec[st][i] - '0';
        if(strVec[st + 1][0] == '$') {
            int strLen = 0;
            for(int i = 1; i < strVec[st].size(); ++i)
                strLen = 10 * strLen + strVec[st + 1][i] - '0';    
                   
        }
    }
}

DbEvent DbReqParser::getEvent()
{
    vector<Slice> strVec;
    int st = splite(strVec, msg);
    DbEvent event;
    parse(0, event, strVec);
    return event;
}





