#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<ctype.h>
#include<sys/socket.h>
#include<pthread.h>
#include<arpa/inet.h>

#define SERV_PORT 9000 
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc,char *argv[])
{
	int lfd = 0, cfd = 0;
	int ret, i;
	char buf[BUFSIZ], client_IP[1024];
	struct sockaddr_in serv_addr, clit_addr;
	socklen_t clit_addr_len;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	lfd = socket(AF_INET, SOCK_STREAM, 0);

	if(lfd == -1)
	{
		sys_err("socket error");
	}
	bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	listen(lfd, 128);
	
	clit_addr_len = sizeof(clit_addr);
	cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);  //此处的clit_addr 为何不需要初始化？
	printf("the clit_addr ip is %s,port is %d\n",
		inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_IP,sizeof(client_IP)),
		ntohs(clit_addr.sin_port));

	if(cfd == -1)
	{
		sys_err("accept error");
	}
	
	while(1)
	{
		ret = read(cfd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf ,ret);

		for(i = 0; i<ret; i++)
		{
			buf[i] = toupper(buf[i]);
		}

		write(cfd, buf ,ret); 
	}
	return 0;
}
