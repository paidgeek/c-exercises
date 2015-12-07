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

int main(int argc, char *argv[])
{
	msgbuf buf;
	int msqid;
	key_t key;

	if((key = ftok("/tmp", 'A')) == -1) {
		PERR("ftok")
	}

	if((msqid = msgget(key, 0644)) == -1) {
		PERR("msgget")
	}

	while(msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) != -1) {
		printf("%s", buf.mtext);
	}

	return 0;
}
