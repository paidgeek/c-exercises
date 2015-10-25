#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFF_SIZE 512
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }

void rev(char *buff, int len)
{
	int i, j;
	char tmp;

	for(i = 0, j = len - 1; i < j; i++, j--) {
		tmp = buff[i];
		buff[i] = buff[j];
		buff[j] = tmp;
	}
}

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
	int n, m, curr, len;

	if((len = (int)lseek(src, 0, SEEK_END)) < 0) {
		ERR("seek error")
	}

	if(lseek(src, 0, SEEK_SET) < 0) {
		ERR("seek error")
	}

	curr = len - 1;
	
	while((n = read(src, buff, BUFF_SIZE)) > 0) {
		if(lseek(dst, curr, SEEK_SET) < 0) {
			ERR("seek error")
		}		

		curr -= n;

		m = n;
		rev(buff, m);
	
		if(write(dst, buff, m) != m) {
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
