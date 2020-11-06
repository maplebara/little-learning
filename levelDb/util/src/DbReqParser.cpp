#include "DbReqParser.h"
#include <unordered_map>
#include <string>

namespace {
    std::unordered_map<std::string, OpType> = { {"open", kAdd}, {"set", kDel}, {"get", kUpdate} };
}

Slice DbReqParser::key()
{
    Slice s_key;

}

Slice DbReqParser::value()
{

}

OpType DbReqParser::ops()
{

}

int DbReqParser::parse()
{
    switch(data[0]) {
        case '$' : 
            return parseString(); 
        case '*' : 
            return parseArray();
        default : 
            printf("Undefined operator!!");
    }
    return 0;
}





