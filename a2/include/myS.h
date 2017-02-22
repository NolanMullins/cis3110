#ifndef __MULLINSN_STRUCTS__
#define __MULLINSN_STRUCTS__

#include "list.h"

typedef struct data
{
	int numProc;
	int threadSwitch;
	int procSwitch;

	List* procs;
} Data;

typedef struct proc
{
	int procNum;
	int numThreads;

	List* threads;
} Proc;

typedef struct thread
{
	int threadNum;
	int arrival;
	int numCPU;

	List* cpus;
} Thread;

typedef struct cpu
{
	int cpuNum;
	int cpuTime;
	int ioTime;
} CPU;

#endif