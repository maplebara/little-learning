#include "common.h"

#include <unistd.h>
#include <stdio.h>

int readAll(int fd, void* buf, size_t len)
{
    char* buff = (char*)buf;
    size_t start = 0;
    size_t totalLen = len;
    ssize_t rlen = 0;

    do {
        rlen = read(fd, buff + start, totalLen);
        start += rlen;
        totalLen = totalLen - rlen;

        if(rlen == -1) 
        {
            perror("read occured error!!\n");
            return -1;
        }
    } while(totalLen > 0);

    return 0;
}

int writeAll(int fd, void* buf, size_t len)
{
    char* buff = (char*)buf;
    size_t start = 0;
    size_t totalLen = len;
    ssize_t wlen = 0;

    do {
        wlen = write(fd, buff + start, totalLen);
        start += wlen;
        totalLen -= wlen;
        if(wlen == -1) 
        {
            perror("write occured error!!\n");
            return -1;
        }
    } while(totalLen > 0);
    return 0;
}

