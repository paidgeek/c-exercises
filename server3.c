#include "server.h"
#include <math.h>
#include <signal.h>
#include "expr.h"

#define PERR(msg) {perror(msg);exit(1);}
#define ERR(msg) {printf("%s\n", msg);exit(1);}
#define MAXLINE 1024
#define PORT 9876
#define LISTENQ 10
#define SA struct sockaddr

void sig_child(int signo)
{
	pid_t pid;
	int stat;
	
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		printf("child %d terminated\n", pid);
	}	
}

void con_handler(int sockfd)
{
	int n;
	char buf[MAXLINE];
	
	again:
	while((n = readline(sockfd, buf, MAXLINE)) > 0) {
		snprintf(buf, MAXLINE, "%f\n", evaluate(buf));
		n = strlen(buf);
		
		if(writen(sockfd, buf, n) != n) {
			PERR("writen")
		}
	}
	
	if(n < 0 && errno == EINTR) {
		goto again;
	} else if(n < 0) {
		ERR("read error")
	}
}

int main(int argc, char *argv[])
{
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	struct sigaction sa;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	
	if(signal(SIGCHLD, sig_child) < 0) {
		PERR("signal")
	}
	
	for(;;) {
		clilen = sizeof(cliaddr);
		
		if((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if(errno == EINTR) {
				continue;
			} else {
				PERR("accept")
			}
		}
		
		if((childpid = fork()) < 0) {
			PERR("fork")
		} else if(childpid == 0) {
			Close(listenfd);
			con_handler(connfd);

			return 0;
		} else {
			Close(connfd);
		}
	}
	
	Close(listenfd);

	return 0;
}
