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

#define SERV_PORT 9000

void catch_child(int signum)
{
   while( waitpid(0, NULL, WNOHANG) > 0);
   
   return;
}

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
int main(int agrc, char *argv[])
{
    int lfd = 0, cfd = 0;
    int ret, i;
    pid_t pid;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len;

    //memset(&serv_addr, 0, sizeof(serv_addr));
    bzero(&serv_addr,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(lfd, 128);

    clit_addr_len = sizeof(clit_addr);

    while(1)
    {
        cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);    
        
        pid = fork();
        if(pid == 0)
        {
            close(lfd);
            while(1)
            {
                ret = read(cfd, buf, sizeof(buf));
                if(ret == 0)
                {
                    close(cfd);
                    exit(1);
                }
                write(STDOUT_FILENO, buf, ret);
    
                for(i = 0; i < ret; i++)
                {
                    buf[i] = toupper(buf[i]);
                }
                write(cfd, buf, ret);
                write(STDOUT_FILENO, buf, ret);
             }
        }
        else
        {
            struct sigaction act;
            
            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;

            sigaction(SIGCHLD, &act, NULL);
            
            close(cfd);

            continue;
        }
    }

    close(lfd);
    close(cfd);

    return 0;
}

