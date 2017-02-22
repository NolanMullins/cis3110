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
	int arrivalB = ((Thread*)a)->arrival;
	return arrivalA-arrivalB;
}

CPU* genCPU(int cpuNum, int cpuTime, int ioTime)
{
	CPU* c = malloc(sizeof(CPU));
	c->cpuNum = cpuNum;
	c->cpuTime = cpuTime;
	c->ioTime = ioTime;
	return c;
}

Thread* parseThread(FILE* f)
{
	int threadNum=0, arrivalTime=0, numCPU=0;
	fscanf(f, "%d %d %d", &threadNum, &arrivalTime, &numCPU);
	readToEnd(f);

	Thread* t = malloc(sizeof(Thread));
	t->threadNum = threadNum;
	t->arrival = arrivalTime;
	t->numCPU = numCPU;
	t->cpus = init();

	for (int a = 0; a < numCPU - 1; a++)
	{
		int cpuNum=0, cpuTime=0, ioTime=0;
		fscanf(f, "%d %d %d", &cpuNum, &cpuTime, &ioTime);
		readToEnd(f);
		listAdd(t->cpus, genCPU(cpuNum, cpuTime, ioTime));
	}
	int cpuNum=0, cpuTime=0;
	fscanf(f, "%d %d", &cpuNum, &cpuTime);
	readToEnd(f);
	listAdd(t->cpus, genCPU(cpuNum, cpuTime, 0));

	return t;
}

Proc* parseProc(FILE* f)
{
	int procNum=0, numThreads=0;
	fscanf(f, "%d %d", &procNum, &numThreads);
	printf("%d %d\n", procNum, numThreads);
	readToEnd(f);

	Proc* p = malloc(sizeof(Proc));
	p->procNum = procNum;
	p->numThreads = numThreads;
	p->threads = init();

	for (int a = 0; a < numThreads; a++)
	{
		//void insertSorted(List* head, int (*cmp)(void* a, void* b), void* data);
		insertSorted(p->threads, cmpThread, parseThread(f));
	}

	return p;
}

Data* parseFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");

	// number_of_processes thread_switch process_switch
	int numProc=0, threadSwitch=0, procSwitch=0;
	fscanf(f, "%d %d %d", &numProc, &threadSwitch, &procSwitch);
	printf("%d %d %d\n", numProc, threadSwitch, procSwitch);
	readToEnd(f);
	
	Data* d = malloc(sizeof(Data));
	d->numProc = numProc;
	d->threadSwitch = threadSwitch;
	d->procSwitch = procSwitch;
	d->procs = init();

	for (int a = 0; a < numProc; a++)
	{
		listAdd(d->procs, parseProc(f));
	}

	return d;
}