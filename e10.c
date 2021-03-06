#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define PERR(msg) { perror(msg); exit(1); }

void sig_handler(int signo)
{
	if(signo == SIGINT) {
		printf("Prejel signal SIGINT, stevilka %d\n", signo);
	}

	exit(0);
}

int main(int argc, char *argv[])
{
	if(signal(SIGINT, sig_handler) == SIG_ERR) {
		ERR("signal error")
	}

	printf("Lovim signal SIGINT ...\n");

	sleep(3);

	printf("Ni bilo signala SIGINT v zadnjih 3 sekundah\n");

	signal(SIGINT, SIG_IGN);
	printf("Naslednje 3 sekunde ignoriram signal SIGINT ...\n");
	sleep(3);

	signal(SIGINT, SIG_DFL);
	printf("Privzeta funkcija za SIGINT ...\n");
	sleep(3);

	printf("Ni bilo signala SIGINT v zadnjih 3 sekundah, zakljucujem.\n");
	
	return 0;
}
