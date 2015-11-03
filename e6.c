#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define PERR(msg) { perror(msg); exit(1); }

int main(int argc, char *argv[])
{
	DIR *dir;

	if((dir = opendir(".")) == NULL) {
		PERR(".")
	}

	struct dirent *de;
	struct stat st;

	while((de = readdir(dir)) != NULL) {
		if(stat(de->d_name, &st) < 0) {
			PERR("stat")
		}

		char info[10] = {[0 ... 9] = '-'};

		switch(st.st_mode & S_IFMT) {
			case S_IFDIR:
				info[0] = 'd';				
				break;
			case S_IFLNK:
				info[0] = 'l';			
				break;
			case S_IFIFO:
				info[0] = 'p';
				break;
			case S_IFSOCK:
				info[0] = 's';				
				break;
			case S_IFBLK:
				info[0] = 'b';
				break;
			case S_IFCHR:
				info[0] = 'c';
				break;
		}

		const int perms[] = {
			S_IRUSR, S_IWUSR, S_IXUSR,
			S_IRGRP, S_IWGRP, S_IXGRP,
			S_IROTH, S_IWOTH, S_IXOTH
		};
		const char *flgs = "rwx";
		int i;

		for(i = 0; i < 9; i++) {
			if(st.st_mode & perms[i]) {
				info[i + 1] = flgs[i % 3];
				break;
			}
		}

		printf("%s %d %s\n", info, st.st_nlink, de->d_name);
	}

	if(closedir(dir) < 0) {
		PERR(".")
	}

	return 0;
}
