#include"wrap.h"

#define SERV_PORT 9000

int main(int agrc, char *argv[])
{
    int lfd = 0, cfd = 0;
    int ret, i;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //创建套接字
    //#include<sys/socket.h>
    //    int socket(int domain, int type, int protocol);
    //    domain: 选用的ip地址协议 AF_INET/AF_INET6/AF_UNIX
    //    type: 创建套接字所选用的数据传输协议  SOCK_STREAM/SOCK_DGRAM
    //    protocol: 所选用协议的代表协议 0(TCP(SOCK_STREAM)/UDP(SOCK_DGRAM))
    //返回值: 成功: 新套接字所对应的文件描述符
    //        失败: -1 errno
    
    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    //给socket绑定一个地址结构(ip+port)
    //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    //    sockfd: 函数返回值
    //    struct sockaddr_in addr:
    //        addr.sin_family = AF_INET;
    //        addr.sin_port = htons(port_number);
    //        addr.sin_addr.s_addr = htonl(INADDR_ANY)
    //    addr:传入参数(struct sockaddr *)addr
    //    addrlen:sizeof(addr) 地址结构的大小
    //返回值: 成功: 0
    //        失败: -1 errno
    
    ret = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if(ret == -1)
    {
        sys_err("bind error");
    }

    //设置同时与服务器建立连接的上限属(同时进行3次握手的客户端数量)
    //int listen(int sockfd, int backlog);
    //    sockfd: socket函数返回值
    //    backlog: 上限数值。最大值128
    //返回值: 成功: 0
    //        失败：-1 errno
    
    ret = listen(lfd, 128);
    if(ret == -1)
    {
        sys_err("listen error");
    }

    //阻塞等待客户端建立连接，成功的话，返回一个与客户端成功连接的socket文件描述符
    //int accept(int sockfd, struct sockaddr *addr, socklen *addrlen);
    //    sockfd: socket 函数返回值
    //    addr: 传出参数 成功与服务器建立连接的那个客户端的地址结构
    //    addrlen: 传入传出 入: addr的大小 出: 客户端addr的实际大小
    //        socklen_t clit_addr_len = sizeof(addr)
    //        &clit_addr——len
    //返回值: 成功: 够与服务器进行数据通信的socket对应的文件描述符
    //        失败: -1 errno
    
    clit_addr_len = sizeof(clit_addr);
    cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);    
    if(cfd == -1)
   
    {
        sys_err("accept error");
    }
    
    while(1)
    {
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
    
        for(i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        
        write(cfd, buf, ret);
    }

    close(lfd);
    close(cfd);

    return 0;
}

