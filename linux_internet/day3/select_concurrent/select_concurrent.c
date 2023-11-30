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


#define SERV_PORT 9000

int main(int argc, char *argv[])
{
    int lfd, cfd, max_fd, ret_num, i, ret_read, j;
    struct sockaddr_in serv_addr, clit_addr;
    fd_set rset, allset;
    char buf[BUFSIZ];

    int maxi = -1, n;
    int client[1024];
    for(n = 0; n < 1024; n++)
    {
        client[n] = -1;
    }

    socklen_t clit_addr_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    clit_addr_len = sizeof(clit_addr_len);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(lfd, 128);

    FD_ZERO(&allset);
    FD_SET(lfd, &allset);
    
    max_fd = lfd;
    
    while(1)
    {    
        rset = allset;
        ret_num = select(max_fd + 1, &rset, NULL, NULL, NULL);
        if(FD_ISSET(lfd, &rset))
        {
            cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
            FD_SET(cfd, &allset);
            
            for(n = 0; n < 1024; n++)
            {
                if(client[n] < 0)
                {
                    client[n] = cfd;
                    break;
                }
            }
            
            if(n > maxi)
                maxi = n;
            if(max_fd < cfd)
                max_fd = cfd;
            
            if((--ret_num) == 0)
                continue;
        }
        
        for(i = 0; i < maxi + 1; i++)
        {   
            
            if(FD_ISSET(client[i], &rset))
            {
                ret_read =  read(client[i], buf, BUFSIZ);
                if(ret_read == 0)
                {
                    close(client[i]);
                    FD_CLR(client[i], &allset);
                }
                for(j = 0; j< ret_read; j++)
                {
                    buf[j] = toupper(buf[j]);
                }
                write(client[i], buf ,ret_read);
                write(STDOUT_FILENO, buf, ret_read); 
            }
        }
    }
    close(lfd);

    return 0;
}


