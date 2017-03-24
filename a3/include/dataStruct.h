#ifndef __MULLINSN_dataStruct__
#define __MULLINSN_dataStruct__

typedef enum state
{
	thinking,
	eating
} State;

typedef struct data 
{
	int tid;
	State state; 
	int hungry;
	int* forks;
	pthread_mutex_t* mutex;
}Data;


#endif