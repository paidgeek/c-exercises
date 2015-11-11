#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFF_SIZE 4
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define MAX(x, y) x > y ? x : y

int main(int argc, char *argv[])
{
	int n = 5;
	char *pathname;

	if(argc > 3) {
		ERR("illegal args")
	}

	while(--argc > 0) {
		char *arg = argv[argc];

		if(arg[0] == '-') {
			char *sw = malloc(2);

			sscanf(arg, "%2s%d", sw, &n);
		} else {
			pathname = arg;
		}
	}

	int fd, m, j, len, cur;
	char buff[BUFF_SIZE];

	if((fd = open(pathname, O_RDONLY)) < 0) {
		perror(pathname);
	}

	if((len = (int)lseek(fd, 0, SEEK_END)) < 0) {
		ERR("seek end error")
	}

	cur = MAX(len - BUFF_SIZE, 0);
	if(lseek(fd, cur, SEEK_SET) < 0) {
		ERR("seek set error")
	}

	int nchunks = len / BUFF_SIZE;
	int chunk = len / nchunks;

/*
	printf("len: %d\n", len);
	printf("nchunks: %d\n", nchunks);
	printf("chunk: %d\n", chunk);
*/

	for(j = 0; j < nchunks; j++) {
		lseek(fd, (nchunks - j - 1) * chunk, SEEK_SET);

		if((m = read(fd, buff, chunk)) != chunk) {
			ERR("write error")
		}

		if(fwrite(buff, sizeof(char), m, stdout) != chunk) {
			ERR("write error")
		}
	}

/*
	while((m = read(fd, buff, BUFF_SIZE)) > 0) {
		if(fwrite(buff, sizeof(char), m, stdout) != m) {
			ERR("write error")
		}

		cur = MAX(cur - BUFF_SIZE, 0);
	
		if(lseek(fd, cur, SEEK_SET) < 0) {
			ERR("seek set error")
		}
	}
*/

	close(fd);

	return 0;
}
