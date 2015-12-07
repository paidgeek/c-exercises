#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sem.h>

#define PERR(msg) { perror(msg); exit(1); }

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} argument;

int main(int argc, char *argv[])
{
	key_t key;
	int semid;

	key = ftok("/tmp", 'A');
	
	if((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
		PERR("semget")
	}

	argument.val = 1;

	if(argc > 1) {
		if(semctl(semid, 0, IPC_RMID, NULL) == -1) {
			PERR("semctl")
		}
	} else {
		if(semctl(semid, 0, SETVAL, argument) == -1) {
			PERR("semctl")
		}
	}

	return 0;
}

