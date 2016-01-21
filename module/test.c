#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define PERR(msg) { perror(msg); exit(1); }

void test(int fd, char *buff, size_t len)
{
	int n;
	
	char *b = "hey\n";

	write(fd, b, strlen(b));
	
	if((n = read(fd, buff, strlen(b))) < 0) {
		PERR("read")
	}
	
	buff[n] = '\0';

	printf("%s", buff);
}

int main(int argc, char *argv)
{
	char buff[100];
	int fd;
	
	if((fd = open("/dev/my_module", O_RDWR)) < 0) {
		PERR("open")
	}
	
	test(fd, buff, 2);
	//test(fd, buff, 4);
	//test(fd, buff, 10);

	if(close(fd) < 0) {
		PERR("close")
	}

	return 0;
}
