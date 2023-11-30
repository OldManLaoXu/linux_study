#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define SREV_PORT 9000
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    int cfd = 0;
    int ret;
    char buf[BUFSIZ];
    int count = 10;
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SREV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1)
    {
        sys_err("socket error");
    }

    //使用现有的socket与服务器建立连接
    //int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    //    sockfd: socket函数返回值
    //    addr: 传入参数 服务器的地址结构
    //    addrlen: 服务器地址结构的大小
    //返回值: 成功: 0
    //        失败: -1 errno
    //如果不使用bind绑定客户端地址结构，采用“隐式绑定”
    
    ret = connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret == -1)
    {
        sys_err("connect error");
    }
    
    while(--count)
    {
        write(cfd, "hello\n", 6);
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        sleep(1);
    }

    close(cfd);
    
    return 0;
}

