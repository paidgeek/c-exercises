#include <stdio.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>

#define PERR(msg) { perror(msg); exit(1); }
#define SHM_SIZE 256

int main(int argc, char *argv[])
{
	key_t key;
	int shmid;
	char *shmaddr;

	key = ftok("/tmp", 'A');

	if((shmid = shmget(key, 0, 0644)) == -1) {
		PERR("shmget")
	}

	if((shmaddr = shmat(shmid, 0, 0)) == NULL) {
		PERR("shmat")
	}

	printf("%s", shmaddr);

	if(shmdt(shmaddr) == -1) {
		PERR("shmdt")
	}

	if(shmctl(shmid, IPC_RMID, 0) == 1) {
		PERR("shmctl")
	}

	return 0;
}

