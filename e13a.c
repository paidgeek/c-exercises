#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define PERR(msg) { perror(msg); exit(1); }
#define FIFO_NAME "pipe"

int main(int argc, char *argv[])
{
	int fd, n;
	char buf[100];

	if(mkfifo(FIFO_NAME, 0666) == -1) {
		PERR(FIFO_NAME)
	}

	if((fd = open(FIFO_NAME, O_WRONLY)) == -1) {
		PERR("open")
	}

	while(fgets(buf, sizeof(buf), stdin) != NULL) {
		n = strlen(buf);
		
		if(write(fd, buf, n) != n) {
			PERR("write")
		}
	}

	if(close(fd) == -1) {
		PERR("close")
	}

	if(unlink(FIFO_NAME) == -1) {
		PERR("unlink")
	}

	return 0;
}
