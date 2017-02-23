#include <stdio.h>
#include <stdlib.h>
#include "myS.h"
#include "list.h"

void readToEnd(FILE* f)
{
	char c = '-';
	while ((c = getc(f)) != '\n' && c != EOF);
}

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

Thread* parseThread(FILE* f)
{
	int threadNum=0, arrivalTime=0, numCPU=0;
	fscanf(f, "%d %d %d", &threadNum, &arrivalTime, &numCPU);
	readToEnd(f);

	Thread* t = malloc(sizeof(Thread));
	t->threadNum = threadNum;
	t->arrival = arrivalTime;
	t->numCPU = numCPU;
	//t->cpus = init();

	for (int a = 0; a < numCPU - 1; a++)
	{
		int cpuNum=0, cpuTime=0, ioTime=0;
		fscanf(f, "%d %d %d", &cpuNum, &cpuTime, &ioTime);
		readToEnd(f);
		t->totCpuTime += cpuTime;
		t->totIoTime += ioTime;
		//listAdd(t->cpus, genCPU(cpuNum, cpuTime, ioTime));
	}
	int cpuNum=0, cpuTime=0;
	fscanf(f, "%d %d", &cpuNum, &cpuTime);
	readToEnd(f);
	t->totCpuTime += cpuTime;
	t->timePool = t->totCpuTime+t->totIoTime;
	//listAdd(t->cpus, genCPU(cpuNum, cpuTime, 0));

	return t;
}

void parseProc(FILE* f, List* threads)
{
	int procNum=0, numThreads=0;
	fscanf(f, "%d %d", &procNum, &numThreads);
	//printf("%d %d\n", procNum, numThreads);
	readToEnd(f);


	for (int a = 0; a < numThreads; a++)
	{	
		Thread* t = parseThread(f);
		t->procNum = procNum;
		//printf("Proc: %d Thread: %d Arrival: %d\n", t->procNum, t->threadNum, t->arrival);
		//void insertSorted(List* head, int (*cmp)(void* a, void* b), void* data);
		insertSorted(threads, cmpThread, t);
	}
}

Data* parseFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");

	// number_of_processes thread_switch process_switch
	int numProc=0, threadSwitch=0, procSwitch=0;
	fscanf(f, "%d %d %d", &numProc, &threadSwitch, &procSwitch);
	//printf("%d %d %d\n", numProc, threadSwitch, procSwitch);
	readToEnd(f);
	
	Data* d = malloc(sizeof(Data));
	d->numProc = numProc;
	d->threadSwitch = threadSwitch;
	d->procSwitch = procSwitch;
	d->threads = init();

	for (int a = 0; a < numProc; a++)
	{
		parseProc(f, d->threads);
	}

	return d;
}