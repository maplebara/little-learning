#include <sys/socket.h>
#include <sys/types.h>
#include "basic/BasicMacro.h"

int open()
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    USI_ASSERT_TRUE(sockfd > 0);

    return 0;
}