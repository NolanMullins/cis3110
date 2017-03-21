#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "list.h"
#include "dataStruct.h"

void *runfor(void *threadid)
{
	return NULL;
}

int main(int argc, char* argv[])
{
	int threadCount = sysconf(_SC_NPROCESSORS_ONLN);
	pthread_t threads[threadCount];
	int rc;
	long t;

	printf("Available threads: %d\n", threadCount);
	for(t=0; t<threadCount; t++)
	{
		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, runfor, (void *)t);
	  	if (rc)
	  	{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	for(int i = 0; i < threadCount; i ++)
	{
		pthread_join(threads[i],NULL);
	}

	/* Last thing that main() should do */
	pthread_exit(NULL);
	return 0;
}