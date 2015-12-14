#include "server.h"
#include <math.h>

#define PERR(msg) {perror(msg);exit(1);}
#define MAXLINE 1024
#define PORT 9876
#define LISTENQ 10
#define SA struct sockaddr

double evaluate(const char *expr)
{
	float a, b;
	char op;
	
	sscanf(expr, "%f %c %f", &a, &op, &b);
	
	switch(op) {
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '*':
			return a * b;
		case '/':
			return a / b;
	}
	
	return NAN;
}

int main(int argc, char *argv[])
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int n;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for(;;) {
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		while((n = readline(connfd, buf, MAXLINE)) != 0) {
			snprintf(buf, MAXLINE, "%f\n", evaluate(buf));
			
			writen(connfd, buf, strlen(buf));
		}

		Close(connfd);
	}

	Close(listenfd);

	return 0;
}
