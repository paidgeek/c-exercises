#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFF_SIZE 512
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }

int main(int argc, char *argv[])
{
	if(argc != 3) {
		ERR("missing arguments")
	}

	int src, dst;

	if((src = open(argv[1], O_RDONLY)) < 0) {
		perror(argv[1]);
	}

	if((dst = creat(argv[2], S_IRUSR | S_IWUSR)) < 0) {
		perror(argv[2]);
	}

	char buff[BUFF_SIZE];
	int n, m;

	while((n = read(src, buff, BUFF_SIZE)) > 0) {
		m = n;

		do {
			n = write(dst, buff, m);
			m -= n;
		} while(m > 0);

		if(m < 0) {
			ERR("write error")
		}
	}

	if(n < 0) {
		ERR("read error")
	}

	close(src);
	close(dst);

	return 0;
}
