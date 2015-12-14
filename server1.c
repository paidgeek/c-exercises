#include "server.h"

#define PERR(msg) {perror(msg);exit(1);}
#define MAXLINE 1024
#define PORT 9876
#define LISTENQ 10
#define SA struct sockaddr

int main(int argc, char *argv[])
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char wbuff[MAXLINE];
	int len, i;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for(;;) {
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		for(i = 1; i <= 5; i++) {
			snprintf(wbuff, sizeof(wbuff), "%d\n", i);
		
			len = strlen(wbuff);
			
			printf("Poslano: %d\n", i);
			
			if(writen(connfd, wbuff, len) != len) {
				PERR("writen")
			}
			
			sleep(1);
		}

		Close(connfd);
	}

	Close(listenfd);

	return 0;
}
