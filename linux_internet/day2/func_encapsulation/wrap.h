#ifndef _WRAP_H_
#define _WRAP_H_
#include <stdio.h>
#include<ctype.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<arpa/inet.h>

void sys_err(const char *str);
int Socket(int domain, int type, int protocol);

#endif
