#include "basic/util.h"
#include <sys/syscall.h>


pid_t GetTid()
{
    return syscall(SYS_gettid);
}

