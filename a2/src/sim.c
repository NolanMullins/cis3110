#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "myS.h"

void detailPrint(int arrival, int serviceTime, int ioTime, int switchTime, int turnaroundTime, int finishTime)
{
	printf("Arrival time: %d\n", arrival);
	printf("Service Time: %d\n", serviceTime);
	printf("I/O Time: %d\n", ioTime);
	printf("Switch Time: %d\n", switchTime);
	printf("Turnaround Time: %d\n", turnaroundTime);
	printf("Finish Time: %d\n", finishTime);
}

void threadDetail(Thread* t, int switchTime, int* totalTime, int detail)
{
	int serviceTime = t->totCpuTime;
	int ioTime = t->totIoTime;
	//int finishTime = 0;
	/*List* cpus = t->cpus;
	for (int c = 0; c < listSize(cpus); c++)
	{
		CPU* cpu = (CPU*)listGet(cpus, c);
		//printf("%d %d\n", cpu->cpuTime, cpu->ioTime);
		serviceTime += cpu->cpuTime;
		ioTime += cpu->ioTime;
	}*/
    
	*totalTime += serviceTime+ioTime+switchTime;

	if (detail)
		//detailPrint(t->arrival, serviceTime, ioTime, switchTime, serviceTime+ioTime+switchTime, *totalTime);
		detailPrint(t->arrival, serviceTime, ioTime, switchTime, *totalTime - t->arrival, *totalTime);
}

void fcfs(Data* data, int detail, int verbose)
{
	//Do shit now
	List* threads = data->threads;
	int switchTime = 0;
	int totalTime = 0;
	int numThreads = listSize(threads);
	//int threadTime = 0;

	int currentSwitch = 0;
	int curProcNum = ((Thread*)listGet(threads, 0))->procNum;
	int curThreadNum = ((Thread*)listGet(threads, 0))->threadNum;

	for (int a = 0; a < listSize(threads); a++)
	{
		Thread* t = (Thread*)listGet(threads, a);

		if (curProcNum != t->procNum)
			currentSwitch = data->procSwitch;
		else if (curThreadNum != t->threadNum)
			currentSwitch = data->threadSwitch;

		curProcNum = t->procNum;
		curThreadNum = t->threadNum;

		if (verbose)
			printf("At time: %d, Thread %d of process %d moves from new to ready\n", totalTime, t->threadNum, t->procNum);
		switchTime += currentSwitch;
		if (verbose)
			printf("At time: %d, Thread %d of process %d moves from ready to running\n", totalTime+currentSwitch, t->threadNum, t->procNum);
		//printf("threadNum: %d, %d\n", t->threadNum, t->arrival);
		if (detail)
			printf("\nProcess: %d, Thread: %d\n",t->procNum, t->threadNum);

		threadDetail(t, currentSwitch, &totalTime, detail);

		if (verbose)
			printf("\nAt time: %d, Thread %d of process %d moves from running to termination\n\n", totalTime, t->threadNum, t->procNum);

		//currentSwitch = data->threadSwitch;
		//currentSwitch = data->procSwitch;
	}
	printf("\n");
	printf("Time: %d\n", totalTime);
	printf("Avg: %.1lf\n", (double)totalTime/(double)numThreads);

	double util = (double)(totalTime-switchTime)/(double)totalTime;
	util *= 100;
	printf("CPU Util: %.1lf%%\n", util);
}

void rrDetail(Thread* t, int switchTime, int* totalTime, int detail, int quantum)
{
	int serviceTime = 0;
	int ioTime = 0;
	//printf("%d %d\n",t->totCpuTime, quantum);
	if (t->totCpuTime > quantum)
	{
		serviceTime = quantum;
		quantum = 0;
		t->totCpuTime -= serviceTime;
	}
	else
	{
		serviceTime = t->totCpuTime;
		quantum -= serviceTime;
		t->totCpuTime = 0;
	}
	
	//printf("%d %d\n",t->totIoTime, quantum);
	if (t->totIoTime > quantum)
	{
		ioTime = quantum;
		quantum = 0;
		t->totIoTime -= ioTime;
	}
	else
	{
		ioTime = t->totIoTime;
		quantum -= ioTime;
		t->totIoTime = 0;
	}
	//printf("%d\n", quantum);
	int runTime = ioTime+serviceTime;
	t->timePool = t->totCpuTime+t->totIoTime;
	*totalTime += runTime+switchTime;
	//*totalTime += serviceTime+ioTime+switchTime;

	if (detail)
		detailPrint(t->arrival, serviceTime, ioTime, switchTime, *totalTime - t->arrival, *totalTime);
}

void  RR(Data* data, int detail, int verbose)
{
	//Do shit now
	List* threads = data->threads;
	int switchTime = 0;
	int totalTime = 0;
	int numThreads = listSize(threads);
	//int threadTime = 0;

	int currentSwitch = 0;
	int curProcNum = ((Thread*)listGet(threads, 0))->procNum;
	int curThreadNum = ((Thread*)listGet(threads, 0))->threadNum;

	while (listSize(threads) > 0)
	{
		Thread* t = (Thread*)listGet(threads, 0);

		if (curProcNum != t->procNum)
			currentSwitch = data->procSwitch;
		else if (curThreadNum != t->threadNum)
			currentSwitch = data->threadSwitch;

		curProcNum = t->procNum;
		curThreadNum = t->threadNum;

		if (verbose)
			printf("At time: %d, Thread %d of process %d moves from new to ready\n", totalTime, t->threadNum, t->procNum);
		switchTime += currentSwitch;
		if (verbose)
			printf("At time: %d, Thread %d of process %d moves from ready to running\n", totalTime+currentSwitch, t->threadNum, t->procNum);
		//printf("threadNum: %d, %d\n", t->threadNum, t->arrival);
		if (detail)
			printf("\nProcess: %d, Thread: %d\n",t->procNum, t->threadNum);

		rrDetail(t, currentSwitch, &totalTime, detail, 100);

		if (t->timePool > 0)
		{
			if (verbose)
				printf("\nAt time: %d, Thread %d of process %d moves from running to blocked\n\n", totalTime, t->threadNum, t->procNum);
			Thread* cur = (Thread*)listRemove(threads, 0);
			listAdd(threads, cur);
		}
		else
		{

			if (verbose)
				printf("\nAt time: %d, Thread %d of process %d moves from running to termination\n\n", totalTime, t->threadNum, t->procNum);
			Thread* cur = (Thread*)listRemove(threads, 0);
			free(cur);
		}
		

		//currentSwitch = data->threadSwitch;
		//currentSwitch = data->procSwitch;
	}
	printf("\n");
	printf("Time: %d\n", totalTime);
	printf("Avg: %.1lf\n", (double)totalTime/(double)numThreads);

	double util = (double)(totalTime-switchTime)/(double)totalTime;
	util *= 100;
	printf("CPU Util: %.1lf%%\n", util);
}