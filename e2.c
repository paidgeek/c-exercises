#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFF_SIZE 512
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }

int main(int argc, char *argv[])
{
	char buff[BUFF_SIZE];
	int n;

	if(argc > 1) {
		int i = 1, fd, m;
		char last;

		do {
			if((fd = open(argv[i], O_RDONLY)) < 0) {
				perror(argv[i]);
				exit(1);
			}

			while((n = read(fd, buff, BUFF_SIZE)) > 0) {
				if((m = write(STDOUT_FILENO, buff, n)) != n) {
					ERR("write error")
				}

				last = buff[m - 1];
			}

			close(fd);
		} while(++i < argc);

		if(last != '\n') {
			printf("\n");
		}
	} else {
		while((n = read(STDIN_FILENO, buff, BUFF_SIZE)) > 0) {
			if(write(STDOUT_FILENO, buff, n) != n) {
				ERR("write error")
			}
		}
	}

	return 0;
}
