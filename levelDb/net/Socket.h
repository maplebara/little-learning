#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int setSockNonBlock(int fd);

int listenSocket(unsigned short port);



