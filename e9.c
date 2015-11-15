#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define PERR(msg) { perror(msg); exit(1); }

int execute(char **args)
{
	pid_t forkpid;
	int status;

	if((forkpid = fork()) < 0) {
		PERR("fork")
	} else if(forkpid == 0) {
		if(execvp(args[0], args) < 0) {
			PERR("execvp")
		}

		exit(0);
	} else {
		if(wait(&status) != forkpid) {
			PERR("wait")
		}

		return WEXITSTATUS(status);
	}
}

void parse(char *line, char *args[])
{
	int i;
	char *tok;

	tok = strtok(line, " ");

	for(i = 0; tok != NULL; i++) {
		args[i] = tok;
		tok = strtok(NULL, " ");
	}

	args[i + 1] = NULL;
}

int main(int argc, char *argv[])
{
	setvbuf(stdin, NULL, _IOLBF, 0);

	do {
		char line[100];
		char *args[10];

		printf("> ");
		
		if(fgets(line, sizeof(line), stdin) == NULL) {
			PERR("stdin")
		}

		strtok(line, "\n");
		parse(line, args);

		if(*args[0] != '\n') {
			execute(args);
		}
	} while(TRUE);

	return 0;
}
