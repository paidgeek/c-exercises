#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define PERR(msg) { perror(msg); exit(1); }

int main(int argc, char *argv[])
{
	int fd;

	if(argc == 1) {
		if((fd = creat("stevila", S_IRUSR | S_IWUSR)) < 0) {
			PERR("stevila")
		}
		
		const int n = 5;
		const float vals[] = {1.1, 2.2, 3.3, 4.4, 5.5};
		
		if(write(fd, &n, sizeof(int)) < 0) {
			PERR("stevila")
		}
		
		if(write(fd, vals, sizeof(float) * n) < 0) {
			PERR("stevila")
		}

		if(close(fd) < 0) {
			PERR("stevila");
		}

		return 0;
	}

	const char *pathname = argv[1];

	if((fd = open(pathname, O_RDONLY)) < 0) {
		PERR(pathname)
	}

	int n;
	float f;

	if(read(fd, &n, sizeof(int)) < 0) {
		PERR(pathname)
	}

	while(n-- >= 0) {
		if(read(fd, &f, sizeof(float)) < 0) {
			PERR(pathname)
		}

		printf("%f\n", f);
	}

	if(close(fd) < 0) {
		PERR(pathname)
	}

	return 0;
}
