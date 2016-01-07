#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define PERR(msg) { perror(msg); exit(1); }

int main(int argc, char *argv)
{
	char buf[100];
	int fd, n;
	
	printf("%d\n", sprintf(buf, "awd"));
	
	printf("%d\n", strlen(buf));
	
	printf("%s\n", buf);
	
	return 0;

	if((fd = open("/dev/my_module", O_RDONLY)) < 0) {
		PERR("open")
	}

	if((n = read(fd, &buf, sizeof(buf))) < 0) {
		PERR("read")
	}
	
	buf[n] = '\0';

	printf("%s", buf);

	if(close(fd) < 0) {
		PERR("close")
	}

	return 0;
}
