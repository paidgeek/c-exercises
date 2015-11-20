#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define PERR(msg) { perror(msg); exit(1); }

int main(int argc, char *argv[])
{
	int fd[2];
	pid_t pid;

	if(pipe(fd) == -1) {
		PERR("pipe")
	}

	if((pid = fork()) < 0) {
		PERR("fork")
	} else if(pid == 0) {
		if(close(fd[1]) == -1) {
			PERR("close")
		}

		dup2(fd[0], STDIN_FILENO);

		execlp("grep", "grep", "paidgeek", NULL);
	} else {
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);

		execlp("ps", "ps", "-ef", NULL);
//		wait(NULL);
	}

	return 0;
}
