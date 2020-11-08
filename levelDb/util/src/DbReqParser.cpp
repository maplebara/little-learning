#include "DbReqParser.h"
#include <unordered_map>
#include <string>
#include <vector>

using std::string;
using std::vector;

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

int DbReqParser::parse(DbEvent& event)
{
    return parseArray(0, event);
}

int DbReqParser::parseSimpleStr(int st, Slice& str)
{
    if(st >= msg.size() || msg[st] != '+') return -1;
    ++st;
    int j = st;
    while(j < msg.size() && msg[j] != '\r') ++j;
    if(j == msg.size()) return -1;
    str = Slice(msg.data() + st, j - st);
    return j + 2;
}

int DbReqParser::parseBlkStr(int st, Slice& str)
{
    if(st >= msg.size() || msg[st] != '$') return -1;
    ++st;
    int j = st, len = 0;
    while(j < msg.size() && msg[j] != '\r') 
    {
        len = 10 * len + msg[j] - '0';
        ++j;
    }
    st += 2;
    if(st + len >= msg.size()) return -1;
    str = Slice(msg.data() + st, len);
    return st + len + 2;
}

int DbReqParser::parseArray(int st, DbEvent& event)
{
    if(st >= msg.size() || msg[st] != '*') return -1;
    int j = st, cnt = 0;
    while(j < msg.size() && msg[j] != '\r') 
    {
        cnt = 10 * cnt + msg[j] - '0';
        ++j;
    }
    st += 2;
    int i = 0;
    if(cnt > 3) return -1;
    while(cnt--) {
        if(st >= msg.size()) return -1;
        if(msg[st] == '+') {
            st = parseSimpleStr(st, event.cmd[i++]);
            if(st < 0) return -1;
        }
        else if(msg[st] == '$')
        {
            st = parseBlkStr(st, event.cmd[i++]);
            if(st < 0) return -1;
        }
    }
    return st;
}





