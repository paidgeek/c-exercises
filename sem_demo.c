#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sem.h>

#define PERR(msg) { perror(msg); exit(1); }

int main(int argc, char *argv[])
{
	key_t key;
	int semid;

	key = ftok("/tmp", 'A');
	
	if((semid = semget(key, 1, 0666)) == -1) {
		PERR("semget")
	}

	struct sembuf ops[1];

	ops[0].sem_num = 0;
	ops[0].sem_op = -1;
	ops[0].sem_flg = 0;

	printf("Zasegam vir\n");

	if(semop(semid, ops, 1) == -1) {
		PERR("semop")
	}

	printf("Uspešno!\n");

	getchar();

	ops[0].sem_num = 0;
	ops[0].sem_op = 1;
	ops[0].sem_flg = 0;

	printf("Povečujem semafor za 1\n");

	if(semop(semid, ops, 1) == -1) {
		PERR("semop")
	}

	printf("Vir sproščen!\n");

	return 0;
}

