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

	if((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		PERR("shmget")
	}

	if((shmaddr = shmat(shmid, 0, 0)) == NULL) {
		PERR("shmat")
	}

	if(fgets(shmaddr, SHM_SIZE, stdin) == NULL) {
		PERR("fgets")
	}

	if(shmdt(shmaddr) == -1) {
		PERR("shmdt")
	}

	return 0;
}

