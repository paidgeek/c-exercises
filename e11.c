#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define PERR(msg) { perror(msg); exit(1); }
#define TRUE 1

const int sigs[] = {
	SIGBUS, SIGPIPE, SIGQUIT
};
int cur_sig = 0;

char *sig_str(int signo)
{
	switch(signo) {
		case SIGBUS:
			return "SIGBUS";
		case SIGPIPE:
			return "SIGPIPE";
		case SIGQUIT:
			return "SIGQUIT";
	}

	return NULL;
}

void sig_handler(int signo)
{
	printf("OTROK: od mame sem prejel signal %s\n", sig_str(signo));
}

void alarm_handler(int signo)
{
	if(signo == SIGALRM) {
		int nextsig = sigs[(cur_sig++ % sizeof(sigs))];

		printf("MAMA: cez 3s posljem signal %s ...\n", sig_str(nextsig));

		alarm(3);
	}
}

int main(int argc, char *argv[])
{
	pid_t forkpid;
	int status;

	if((forkpid = fork()) < 0) {
		PERR("fork")
	} else if(forkpid == 0) {
		if(signal(SIGBUS, sig_handler) == SIG_ERR) {
			PERR("signal")
		}

		while(TRUE);
	} else {
		if(signal(SIGALRM, alarm_handler) == SIG_ERR) {
			PERR("signal")
		}

		alarm_handler(SIGALRM);

		if(wait(&status) != forkpid) {
			PERR("wait")
		}
	}

	return 0;
}
