#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

#define PERR(msg) { perror(msg); exit(1); }

typedef struct
{
	long mtype;
	char mtext[200];
} msgbuf;

int msqid;

void bye()
{
	if(msgctl(msqid, IPC_RMID, NULL) == -1) {
		PERR("msgctl")
	}
}

int main(int argc, char *argv[])
{
	atexit(bye);

	msgbuf buf;
	key_t key;

	if((key = ftok("/tmp", 'A')) == -1) {
		PERR("ftok")
	}

	if((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
		PERR("msgget")
	}

	buf.mtype = 1;

	while(fgets(buf.mtext, sizeof(buf.mtext), stdin) != NULL) {
		int len = strlen(buf.mtext);
		
		if(msgsnd(msqid, &buf, len + 1, 0) == -1) {
			PERR("msgsnd")
		}
	}

	return 0;
}
