#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define PERR(msg) { perror(msg); exit(1); }
#define TRUE 1
#define N_THREADS 5
#define EPSILON 0.0001

typedef unsigned char bool;

unsigned int seed;
double N, Z;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

bool
done()
{
	return fabs(4.0 * Z / N - M_PI) < EPSILON;
}

void
*routine_a(void *arg)
{
	const int n = 1000;
	int z, i;
	double x, y;

	while(TRUE) {
		z = 0;

		for(i = 0; i < n; i++) {
			x = ((double) rand_r(&seed)) / RAND_MAX;
			y = ((double) rand_r(&seed)) / RAND_MAX;

			if(x * x + y * y <= 1.0) {
				z++;
			}
		}

		pthread_mutex_lock(&mutex);

		N += n;
		Z += z;

		if(done()) {
			pthread_cond_signal(&cv);
			pthread_mutex_unlock(&mutex);

			break;
		}

		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

void
*routine_b(void *arg)
{
	pthread_mutex_lock(&mutex);

	while(!done()) {
		pthread_cond_wait(&cv, &mutex);
	}

	pthread_mutex_unlock(&mutex);

	double pi = 4.0 * Z / N;
	printf("%-5s %f\n%-5s %f\n", "Pi:", pi, "Real:", M_PI);

	pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
	struct timespec myTime;

	clock_gettime(CLOCK_REALTIME, &myTime);
	seed = myTime.tv_nsec;

	pthread_t ids[N_THREADS];
	pthread_t bid;
	pthread_attr_t attr;
	int i, ret;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&bid, &attr, routine_b, NULL);

	for(i = 0; i < N_THREADS; i++) {
		pthread_create(&ids[i], &attr, routine_a, NULL);
	}

	for(i = 0; i < N_THREADS; i++) {
		pthread_join(ids[i], NULL);
	}

	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cv);

	pthread_exit(NULL);
}

