#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "myS.h"

//just prints that stuff out with a decent format
void detailPrint(int arrival, int serviceTime, int ioTime, int switchTime, int turnaroundTime, int finishTime)
{
	printf("\nArrival time: %d\n", arrival);
	printf("Service Time: %d\n", serviceTime);
	printf("I/O Time: %d\n", ioTime);
	printf("Turnaround Time: %d\n", turnaroundTime);
	printf("Finish Time: %d\n", finishTime);
}
//Run a thread on the cpu
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
	t->ta = *totalTime - t->arrival;
	if (detail)
		//detailPrint(t->arrival, serviceTime, ioTime, switchTime, serviceTime+ioTime+switchTime, *totalTime);
		detailPrint(t->arrival, serviceTime, ioTime, switchTime, *totalTime - t->arrival, *totalTime);
}
//checks is a new process has 'arrived' in the ready queue
void fcfsVerboseReady(List* threads, int totalTime)
{
	for (int b = 0; b < listSize(threads); b++)
	{
		Thread* t = (Thread*)listGet(threads, b);
		//printf("%d %d %d\n", t->procNum, t->threadNum, t->arrival);
		if (t->arrival <= totalTime && t->ready == 0)
		{
			printf("\nAt time: %d, Thread %d of process %d moves from new to ready\n", t->arrival, t->threadNum, t->procNum);
			t->ready = 1;
		}
	}
}

//First come first serve
void fcfs(Data* data, int detail, int verbose)
{
	List* threads = data->threads;
	int switchTime = 0;
	int totalTime = 0;
	int numThreads = listSize(threads);
	//int threadTime = 0;

	int currentSwitch = 0;
	int curProcNum = ((Thread*)listGet(threads, 0))->procNum;
	int curThreadNum = ((Thread*)listGet(threads, 0))->threadNum;

	double ioTime = 0;
	double taTime = 0;
	//loop through the threads and run them
	for (int a = 0; a < listSize(threads); a++)
	{
		Thread* t = (Thread*)listGet(threads, a);
		ioTime += t->ioWork;
		if (curProcNum != t->procNum)
			currentSwitch = data->procSwitch;
		else if (curThreadNum != t->threadNum)
			currentSwitch = data->threadSwitch;
		else 
			currentSwitch = 0;

		curProcNum = t->procNum;
		curThreadNum = t->threadNum;

		if (verbose)
			fcfsVerboseReady(threads, totalTime);

		//if (verbose)
			//printf("At time: %d, Thread %d of process %d moves from new to ready\n", totalTime, t->threadNum, t->procNum);
		switchTime += currentSwitch;
		if (verbose)
			printf("\nAt time: %d, Thread %d of process %d moves from ready to running\n", totalTime+currentSwitch, t->threadNum, t->procNum);
		//printf("threadNum: %d, %d\n", t->threadNum, t->arrival);
		if (detail)
			printf("\nProcess: %d, Thread: %d\n",t->procNum, t->threadNum);
		//run the current thread
		threadDetail(t, currentSwitch, &totalTime, detail);

		if (verbose)
			fcfsVerboseReady(threads, totalTime);

		if (verbose)
			printf("\nAt time: %d, Thread %d of process %d moves from running to termination\n", totalTime, t->threadNum, t->procNum);

		taTime += t->ta;
		//currentSwitch = data->threadSwitch;
		//currentSwitch = data->procSwitch;
	}
	printf("\n");
	printf("Time: %d\n", totalTime);
	//printf("Avg: %.1lf\n", (double)totalTime/(double)numThreads);
	printf("Avg Turnaround: %.1lf\n", taTime/(double)numThreads);
	

	double util = (double)(totalTime-(switchTime+ioTime))/(double)(totalTime);
	util *= 100;
	printf("CPU Util: %.1lf%%\n", util);
}

void rrDetail(Thread* t, int switchTime, int* totalTime, int detail, int quantum)
{
	int serviceTime = 0;
	int ioTime = 0;
	//printf("%d %d\n",t->totCpuTime, quantum);

	//allow the cpu to do work on a quantum amt of the work total
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
	if (t->timePool == 0)
		t->ta = *totalTime - t->arrival;
	//if the thread is done
	if (detail && t->timePool == 0)
	{
		printf("\nProcess: %d, Thread: %d\n",t->procNum, t->threadNum);
		detailPrint(t->arrival, t->cpuWork, t->ioWork, switchTime, *totalTime - t->arrival, *totalTime);
	}
}

void  RR(Data* data, int detail, int verbose, int quantum)
{
	List* threads = data->threads;
	int switchTime = 0;
	int totalTime = 0;
	int numThreads = listSize(threads);
	//int threadTime = 0;

	int currentSwitch = 0;
	int curProcNum = ((Thread*)listGet(threads, 0))->procNum;
	int curThreadNum = ((Thread*)listGet(threads, 0))->threadNum;

	double ioTime = 0;
	double taTime = 0;
	//gather the amount of ioWork to be done
	for (int a = 0; a < listSize(threads); a++)
	{
		Thread* t = (Thread*)listGet(threads, a);
		ioTime += t->ioWork;
	}
	//loop while there are processes that need to be run
	while (listSize(threads) > 0)
	{
		Thread* t = (Thread*)listGet(threads, 0);
		if (curProcNum != t->procNum)
			currentSwitch = data->procSwitch;
		else if (curThreadNum != t->threadNum)
			currentSwitch = data->threadSwitch;
		else
			currentSwitch = 0;

		curProcNum = t->procNum;
		curThreadNum = t->threadNum;
		if (verbose)
			fcfsVerboseReady(threads, totalTime);
		//if (verbose)
			//printf("At time: %d, Thread %d of process %d moves from new to ready\n", totalTime, t->threadNum, t->procNum);
		switchTime += currentSwitch;
		if (verbose)
			printf("\nAt time: %d, Thread %d of process %d moves from ready to running\n", totalTime+currentSwitch, t->threadNum, t->procNum);
		
		//run the thread
		rrDetail(t, currentSwitch, &totalTime, detail, quantum);

		if (verbose)
			fcfsVerboseReady(threads, totalTime);

		//if the thread still has work left
		if (t->timePool > 0)
		{
			if (verbose)
				printf("\nAt time: %d, Thread %d of process %d moves from running to blocked\n", totalTime, t->threadNum, t->procNum);
			Thread* cur = (Thread*)listRemove(threads, 0);
			listAdd(threads, cur);
		}
		//if the thread is done
		else
		{
			//get rid of it
			if (verbose)
				printf("\nAt time: %d, Thread %d of process %d moves from running to termination\n", totalTime, t->threadNum, t->procNum);
			Thread* cur = (Thread*)listRemove(threads, 0);
			taTime += cur->ta;
			free(cur);
		}
	}

	printf("\n");
	printf("Time: %d\n", totalTime);
	//printf("Avg: %.1lf\n", (double)totalTime/(double)numThreads);
	printf("Avg Turnaround: %.1lf\n", taTime/(double)numThreads);

	double util = (double)(totalTime-(switchTime+ioTime))/(double)totalTime;
	util *= 100;
	printf("CPU Util: %.1lf%%\n", util);
}