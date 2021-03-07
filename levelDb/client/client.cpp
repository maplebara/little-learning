#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cassert>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <sstream>

const uint16_t port = 12356;
const uint32_t BUF_LEN = 16 * 1024;

using namespace std;

int dbConnect()
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(sockfd > 0);

    sockaddr_in serverAddr;
    memset(&serverAddr, 0x0, sizeof(sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int status = connect(sockfd, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
    assert(status == 0);

    return sockfd;
}

int main()
{
    int fd = dbConnect();
    assert(fd > 0);
    // char buf[1024] = "*5\r\n$3\r\nset\r\n$13\r\njacipat dante\r\n$15\r\nnerr and virgil\r\n$3\r\nget\r\n$13\r\njacipat dante\r\n";
    // int len = write(fd, buf, sizeof buf);
    // assert(len == sizeof(buf));
    char r_buf[1024] = {0};

    ifstream fin("query.txt");
    std::stringstream buffer;
    buffer << fin.rdbuf();
    string str(buffer.str());
    printf("%lu %s\n", str.size(),buffer.str().c_str());
    int len = write(fd, str.c_str(), str.size());
    int rlen = read(fd, r_buf, 1024);
    printf("%d   %s\n", rlen, r_buf);
    

    while(cin) {
        string str;
        cin >> str; 
        if(str == "exit") {
            close(fd);
        }
    }
    return 0;
}


