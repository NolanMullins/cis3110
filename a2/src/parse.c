#include <stdio.h>
#include <stdlib.h>
#include "myS.h"
#include "list.h"

//this is used to remove comments n stuff
void readToEnd(FILE* f)
{
	char c = '-';
	while ((c = getc(f)) != '\n' && c != EOF);
}

//comparison function used for sorting
int cmpThread(void* a, void* b)
{
	int arrivalA = ((Thread*)a)->arrival;
	int arrivalB = ((Thread*)b)->arrival;
	return arrivalA-arrivalB;
}

/*CPU* genCPU(int cpuNum, int cpuTime, int ioTime)
{
	CPU* c = malloc(sizeof(CPU));
	c->cpuNum = cpuNum;
	c->cpuTime = cpuTime;
	c->ioTime = ioTime;
	return c;
}*/

//parse a thread
Thread* parseThread(FILE* f)
{
	int threadNum=0, arrivalTime=0, numCPU=0;
	fscanf(f, "%d %d %d", &threadNum, &arrivalTime, &numCPU);
	readToEnd(f);

	Thread* t = malloc(sizeof(Thread));
	t->threadNum = threadNum;
	t->arrival = arrivalTime;
	t->numCPU = numCPU;
	t->totIoTime = 0;
	t->totCpuTime = 0;
	//t->cpus = init();
	//parse out the cpus into the thread
	for (int a = 0; a < numCPU - 1; a++)
	{
		int cpuNum=0, cpuTime=0, ioTime=0;
		fscanf(f, "%d %d %d", &cpuNum, &cpuTime, &ioTime);
		readToEnd(f);
		t->totCpuTime += cpuTime;
		t->totIoTime += ioTime;
		//listAdd(t->cpus, genCPU(cpuNum, cpuTime, ioTime));
	}
	//parse the last cpu line
	int cpuNum=0, cpuTime=0;
	fscanf(f, "%d %d", &cpuNum, &cpuTime);
	readToEnd(f);
	t->totCpuTime += cpuTime;
	t->ioWork = t->totIoTime;
	t->cpuWork = t->totCpuTime;
	t->timePool = t->totCpuTime+t->totIoTime;
	t->ready = 0;
	//listAdd(t->cpus, genCPU(cpuNum, cpuTime, 0));

	return t;
}
//parse a process
void parseProc(FILE* f, List* threads)
{
	int procNum=0, numThreads=0;
	fscanf(f, "%d %d", &procNum, &numThreads);
	//printf("%d %d\n", procNum, numThreads);
	readToEnd(f);
	//parse the threads
	for (int a = 0; a < numThreads; a++)
	{	
		Thread* t = parseThread(f);
		t->procNum = procNum;
		//printf("Proc: %d Thread: %d Arrival: %d\n", t->procNum, t->threadNum, t->arrival);
		//void insertSorted(List* head, int (*cmp)(void* a, void* b), void* data);
		insertSorted(threads, cmpThread, t);
	}
}
//start of parsing processes
Data* parseFile(char* fileName)
{
	//FILE* f = fopen(fileName, "r");
	FILE* f = stdin;
	// number_of_processes thread_switch process_switch
	int numProc=0, threadSwitch=0, procSwitch=0;
	//get general data
	fscanf(f, "%d %d %d", &numProc, &threadSwitch, &procSwitch);
	//printf("%d %d %d\n", numProc, threadSwitch, procSwitch);
	readToEnd(f);
	
	Data* d = malloc(sizeof(Data));
	d->numProc = numProc;
	d->threadSwitch = threadSwitch;
	d->procSwitch = procSwitch;
	d->threads = init();
	//parse the processes
	for (int a = 0; a < numProc; a++)
	{
		parseProc(f, d->threads);
	}

	return d;
}