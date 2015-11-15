#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define PERR(msg) { perror(msg); exit(1); }

void pr_exit(int status)
{
	if (WIFEXITED(status))
		printf("normal termination, exit status = %d\n",
		WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n",
		WTERMSIG(status),
		WCOREDUMP(status) ? " (core file generated)" : "");
	else if (WIFSTOPPED(status))
		printf("child stopped, signal number = %d\n",
		WSTOPSIG(status));
}

int main(int argc, char *argv[])
{
	pid_t mypid, forkpid;
	int status;

	if ( (forkpid = fork()) < 0)
		ERR("fork error")
	else if (forkpid == 0) /* child */
		exit(7);
	if (wait(&status) != forkpid) /* wait for child */
		ERR("wait error")

	pr_exit(status); /* and print its status */

	return 0;
}
