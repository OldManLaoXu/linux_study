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
int main(int argc,char *argv[])
{
	int cfd, ret;
	int cont = 10;
	char buf[BUFSIZ];

	struct sockaddr_in serv_addr;  //服务器地址结构
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, "192.168.106.128", &serv_addr.sin_addr.s_addr);

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	//检查返回值。

	connect(cfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));

	while(--cont)
	{
		write(cfd, "hello", 5);
		ret = read(cfd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, ret);
		printf("\n");
		sleep(1);
	}

}
