#include "DbServer.h"
#include "Socket.h"
#include <event2/event.h>
#include <string>

extern DbServer server;

int Client::processQuery()
{
    int originLen = queryBuff.size();
    queryBuff.resize(originLen + QUERY_BUFF_LEN);
    ssize_t len = ::read(fd, &queryBuff.front(), QUERY_BUFF_LEN);
    if(len > 0) {
        queryBuff.resize(originLen + len);
        if(!qryType) {
            qryType = queryBuff[0] == '*' ? 2 : 1;
        }
        if(qryType == 2) {
            if(parseBulkQuery() < 0) return -1;
        }
        else {
            if(parseQuery() < 0) return -1;
        }
    } else {
        exit();
        return -1;
    }
    return 0;
}

int Client::parseQuery()
{
    auto pos = queryBuff.find('\n', qry_pos);
    if(pos == std::string::npos) {
        return -1;
    }
    for(int i = 0; i < (int)pos && queryBuff[pos] != '\r'; ++i) {
        if(queryBuff[i] == ' ') continue;
        int j = i;
        while(j < pos && (queryBuff[j] != ' ' && queryBuff[j] != '\r')) ++j;
        paras.push_back(queryBuff.substr(i, j - i));
        i = j;
    }
    queryBuff.erase(0, pos + 1);
    qryType = 0;
    return !paras.empty() ? 0 : -1;
}

int Client::parseBulkQuery()
{
    size_t pos = 0;
    if(!multiBulkCnt) {
        pos = queryBuff.find('\n');
        if(pos == std::string::npos) {
            return -1;
        }
        if(pos > 1) {
            if(queryBuff[pos - 1] == '\r') multiBulkCnt = stoi(queryBuff.substr(1, pos - 2));
            else multiBulkCnt = stoi(queryBuff.substr(1, pos - 1));
        }
        qry_pos = pos + 1;
    }
    if(qry_pos >= queryBuff.size()) return -1;
    while(multiBulkCnt > 0) {
        if(bulkLen == -1) {
            pos = queryBuff.find('\n', qry_pos);
            if(pos == std::string::npos) {
                return -1;
            }
            if(pos - qry_pos > 1) {
                if(queryBuff[pos - 1] == '\r') bulkLen = stoi(queryBuff.substr(qry_pos + 1, pos - qry_pos - 1));
                else bulkLen = stoi(queryBuff.substr(qry_pos + 1, pos - qry_pos));
            }
            qry_pos = pos + 1;
        }
        if(qry_pos + bulkLen + 2 >= queryBuff.size()) return -1;
        paras.push_back(queryBuff.substr(qry_pos, bulkLen));
        qry_pos += bulkLen + 2;
        --multiBulkCnt;
        bulkLen = -1;
    }
    if(multiBulkCnt == 0) {
        queryBuff.erase(0, qry_pos);
        qry_pos = 0;
        qryType = 0;
        return 0;
    }
    return -1;
}

void reReply(int fd, short event_type, void* evBase)
{
    auto client = server.getClient(fd);
    auto& output = client->getOutputBuff();
    if(!output.empty()) {
        std::unique_lock<std::mutex>(client->getLock());
        int st = write(fd, output.c_str(), output.size());
        if(st > 0) {
            output.erase(0, st);
            if(!output.empty()) client->pendReply((event_base*)evBase);
        } else if(st < 0 && errno == EAGAIN){
            client->pendReply((event_base*)evBase);
        }
        else {
            server.freeClient(fd);
            close(fd);
        }
    }
}

