#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define PERR(msg) { perror(msg); exit(1); }
#define FIFO_NAME "pipe"

int main(int argc, char *argv[])
{
	int fd, n;
	char buf[100];

	if((fd = open(FIFO_NAME, O_RDONLY)) == -1) {
		PERR("open")
	}

	while((n = read(fd, buf, sizeof(buf))) != 0) {
		if(write(STDOUT_FILENO, buf, n) != n) {
			PERR("write")
		}		
	}

	if(n == -1) {
		PERR("read")
	}

	return 0;
}
