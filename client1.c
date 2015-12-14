#include "server.h"

#define PERR(msg) {perror(msg);exit(1);}
#define MAXLINE 1024
#define LISTENQ 10
#define PORT 9876
#define SA struct sockaddr

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;
	char rbuf[MAXLINE];
	int n;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);

	Connect(sockfd, (SA*) &servaddr, sizeof(servaddr));

	n = -1;

	while(n != 0) {
		if((n = readline(sockfd, rbuf, MAXLINE)) < 0) {
			PERR("readline")
		}
		
		printf("Prebrano: %s", rbuf);
	}

	Close(sockfd);

	return 0;
}
