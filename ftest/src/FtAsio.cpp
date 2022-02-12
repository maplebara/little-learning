#include "gtest/gtest.h"
#include <boost/asio.hpp>
#include <thread>
#include <string>
#include <arpa/inet.h>

using namespace boost;
using std::string;

class FtAsio : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {  }
};

void doHandle(asio::io_context* io) {
    io->post([](){printf("post function\n");});
}

void dosting(asio::ip::udp::socket* s)
{
    auto str = "hello";
    char* data = new char[10];
    memcpy(data, str, 6);
    s->async_send(asio::buffer(data, sizeof data), [s](const boost::system::error_code& ec, std::size_t len){
        printf("send msg success.\n");
        sleep(3);
        dosting(s);
    });
}

void handler(asio::io_context* io, asio::deadline_timer* timer)
{
    printf("begin\n");

    doHandle(io);
    sleep(1);

    timer->expires_from_now(posix_time::seconds(2));
    timer->async_wait([io, timer](const system::error_code& error){
        handler(io, timer);
    });
    printf("end\n"); 
}


TEST_F(FtAsio, DISABLED_asio_post_func)
{
    asio::io_service io;
    asio::deadline_timer t1(io);

    auto ep = asio::ip::udp::endpoint(asio::ip::address().from_string("127.0.0.1"), 12000);
    asio::ip::udp::socket s(io);
    auto w = asio::io_service::work(io);

    std::thread t([&io]{io.run(); });
    s.async_connect(ep, [](const boost::system::error_code& ec){printf("connect success."); });
    dosting(&s);

    while(true) {
        io.post([]{printf("test\n");});
        sleep(1);
    }

    t.join();
    printf("t end\n");
}

TEST_F(FtAsio, net_ip_test)
{
    string ip("12.0.0.1");
    string domain("");

    in_addr_t ipNum = inet_addr(ip.c_str());
    if(ipNum == INADDR_NONE) {
        printf("not be valid ip\n");
    } else {
        printf("be valid ip\n");
    }
}


