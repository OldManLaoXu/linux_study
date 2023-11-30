#include "wrap.h"

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int Socket(int domain, int type, int protocol)
{
    int ret;
    ret = socket(domain, type, protocol);
    if(ret == -1)
    {
        sys_err("socket error");
        return ret;    
    }

    return ret;
}


