#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "list.h"
#include "dataStruct.h"

void *phil(void *param)
{
	Data* d = (Data*)param;
	printf("Philosopher %d thinking\n", d->tid);
	while (d->hungry > 0)
	{
		int forks = 0;
		pthread_mutex_lock(d->mutex);
			if (*(d->forks) > 1)
			{
				*(d->forks) -= 2;
				forks = 2;
			}
		pthread_mutex_unlock(d->mutex);
		if (forks > 1)
		{
			printf("Philosopher %d eating\n", d->tid);
			sleep(1);
			d->hungry -= 1;
			pthread_mutex_lock(d->mutex);
				*(d->forks) += forks;
			pthread_mutex_unlock(d->mutex);
			printf("Philosopher %d thinking\n", d->tid);
		}
	}
	free(d);
	return NULL;
}

int main(int argc, char* argv[])
{

	int numP = 4;
	int numE = 2;
	//int threadCount = sysconf(_SC_NPROCESSORS_ONLN);

	pthread_t threads[numP];
	int rc;
	long t;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	//printf("Available threads: %d\n", threadCount);
	for(t=0; t<numP; t++)
	{
		//printf("In main: creating thread %ld\n", t);
		Data* myData = malloc(sizeof(Data));
		myData->tid = t;
		myData->hungry = numE;
		myData->state = thinking;
		myData->forks = &numP;
		myData->mutex = &mutex;
		rc = pthread_create(&threads[t], NULL, phil, (void *)myData);
	  	if (rc)
	  	{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	for(int i = 0; i < numP; i ++)
	{
		pthread_join(threads[i],NULL);
	}

	pthread_exit(NULL);
	return 0;
}