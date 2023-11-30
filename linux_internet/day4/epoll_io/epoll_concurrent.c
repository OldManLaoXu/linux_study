#include <stdio.h>
#include<ctype.h>
#include<sys/socket.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>


#define SERV_PORT 9000

int main(int argc, char *argv[])
{
    int lfd, cfd, i, ret, j;
    struct sockaddr_in serv_addr, clit_addr;
    char buf[BUFSIZ];

    socklen_t clit_addr_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    clit_addr_len = sizeof(clit_addr_len);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(lfd, 128);

    ssize_t efd, res, nready;
    struct epoll_event tep, ep[1024];

    efd = epoll_create(1024);
    tep.events = EPOLLIN;
    tep.data.fd = lfd;
    res = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &tep);
    while(1)
    {    
        nready = epoll_wait(efd, ep, 1024, -1);
        
        for(i = 0; i < nready; i++)
        {
            if(ep[i].data.fd == lfd)
            {
                clit_addr_len = sizeof(clit_addr);
                cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
                tep.data.fd = cfd;
                epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &tep);
            }
            else
            {
                if((ret = read(ep[i].data.fd, buf, sizeof(buf))) == 0)
                {
                    //epoll_ctl(efd, EPOLL_CTL_DEL, ep[i].data.fd, NULL);
                    //close(ep[i].data.fd);
                }
                else
                {
                    for(j = 0; j < ret; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    write(ep[i].data.fd, buf ,ret);
                    write(STDOUT_FILENO, buf, ret); 
                }
            }
        }
    }
    close(lfd);

    return 0;
}


