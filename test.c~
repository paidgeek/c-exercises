#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N_THR 3
#define TCOUNT 10
#define COUNT_LIMIT 12
int count = 0;
int id[3] = {0,1,2};
pthread_mutex_t count_mutex;
pthread_cond_t count_cv;


void *inc_count(void *t)
{
	int i;
	long my_id = (long)t;

	for (i=0; i<TCOUNT; i++) {
		pthread_mutex_lock(&count_mutex);
		count++;

		if (count == COUNT_LIMIT) {
			pthread_cond_signal(&count_cv);
		}

		pthread_mutex_unlock(&count_mutex);

		sleep(1);
	}

	pthread_exit(NULL);
}


void *watch_count(void *t)
{
	pthread_mutex_lock(&count_mutex);

	while (count < COUNT_LIMIT) {
		pthread_cond_wait(&count_cv, &count_mutex);
	}

	pthread_mutex_unlock(&count_mutex);

	printf("%d\n", count);

	pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
int i;
int t1=1, t2=2, t3=3;
pthread_t id[3];
pthread_attr_t attr;
// init
pthread_mutex_init(&count_mutex, NULL);
pthread_cond_init (&count_cv, NULL);
// zaradi prenosljivosti ustvarimo niti kot zdruzljive
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
pthread_create(&id[0], &attr, watch_count, (void *)t1);
pthread_create(&id[1], &attr, inc_count, (void *)t2);
pthread_create(&id[2], &attr, inc_count, (void *)t3);
for (i=0; i<N_THR; i++)
pthread_join(id[i], NULL);
printf ("Main(): %d niti. Konec.\n", N_THR);
pthread_attr_destroy(&attr);
pthread_mutex_destroy(&count_mutex);
pthread_cond_destroy(&count_cv);
pthread_exit(NULL);
} 


