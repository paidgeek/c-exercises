#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define PERR(msg) { perror(msg); exit(1); }

#define A_NAME "izhod1"
#define B_NAME "izhod2"
#define N 1000000

FILE *fopene(const char *pathname, const char *type)
{
	FILE *fp;

	if((fp = fopen(pathname, type)) == NULL) {
		PERR(pathname)
	}

	return fp;
}

time_t test(FILE *fp)
{
	const char sent[] = "hello world\n";
	const int n = sizeof(sent) / sizeof(char);

	int i;	
	time_t t;

	t = time(NULL);

	for(i = 0; i < N; i++) {
		if(fwrite(sent, sizeof(char), n, fp) != n) {
			PERR("fwrite")
		}
	}

	return time(NULL) - t;
}

int main(int argc, char *argv[])
{
	FILE *a, *b;
	char abuf[10], bbuf[100];

	a = fopene(A_NAME, "w");
	b = fopene(B_NAME, "w");

	if(setvbuf(a, abuf, _IOFBF, sizeof(abuf)) != 0) {
		PERR("setvbuf")
	}

	if(setvbuf(b, bbuf, _IOFBF, sizeof(bbuf)) != 0) {
		PERR("setvbuf")
	}

	time_t at, bt;

	at = test(a);
	bt = test(b);

	printf("%s: %ld\n", A_NAME, at);
	printf("%s: %ld\n", B_NAME, bt);

	if(fclose(a) == EOF) {
		PERR(A_NAME)
	}

	if(fclose(b) == EOF) {
		PERR(B_NAME)
	}

	return 0;
}
