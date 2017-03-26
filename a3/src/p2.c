
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "list.h"

typedef struct proc
{
	char id;
	int size;
	int timesSwapped;
}Proc;

//this is used to remove shit i dont want
void readToEnd(FILE* f)
{
	char c = '-';
	while ((c = getc(f)) != '\n' && c != EOF);
}

List* loadProc(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (f == NULL)
		throwError("File does not exist");
	char procID = '\0';
	int procSize = 0;
	List* procData = init();
	while (fscanf(f, "%c %d", &procID, &procSize) > 0)
	{
		readToEnd(f);
		Proc* p = malloc(sizeof(Proc));
		p->id = procID;
		p->size = procSize;
		p->timesSwapped = 0;
		listAdd(procData, p);
		procSize = 0;
	}
	/*for (int a = 0; a < listSize(procData); a++)
		printf("%c %d\n", ((Proc*)listGet(procData, a))->id, ((Proc*)listGet(procData, a))->size);*/
	return procData;
}

int removeProc(char* mem, char id)
{
	for (int a = 0; a < 128; a++)
	{
		int b = a;
		while (mem[b] == id)
		{
			mem[b++] = '\0';
		}
		if (b!=a)
			return a;
	}
	return 0;
}

int addProc(Proc* p, char* mem)
{
	//printf("%c %d\n", p->id, p->size);
	for (int a = 0; a < 128-p->size+1; a++)
	{
		int flag = 0;
		for (int b = a; b < a+p->size; b++)
		{
			if (mem[b] != '\0')
				flag = 1;
		}
		if (flag)
			continue;
		//add proc to memory
		for (int b = a; b < a+p->size; b++)
		{
			mem[b] = p->id;
		}
		return a;
	}
	return -1;
}

void freeProc(void* data)
{
	free((Proc*)data);
}

int numHoles(char* mem)
{
	int holes = 0;
	int flag = 1;
	for (int a = 0; a < 128; a++)
	{
		if (flag)
		{
			if (mem[a] == '\0')
				flag = 0;
		}
		else if (mem[a] != '\0')
		{
			flag = 1;
			holes++;
		}
	}
	if (flag == 0)
		holes++;
	return holes;
}

int memInUse(char* mem)
{
	int inUse = 0;
	for (int a = 0; a < 128; a++)
		if (mem[a] != '\0')
			inUse++;
	return inUse;
}

void firstFist(char* fileName)
{
	char mem[128];
	List* procData = loadProc(fileName);
	List* inMem = init();

	int loads = 0;
	int avgProcs = 0;
	int avgHoles = 0;
	float memUsage = 0;


	for (int a = 0; a < 128; a++)
		mem[a] = '\0';
	while (listSize(procData) > 0)
	{
		loads++;
		Proc* p = (Proc*)listRemove(procData, 0);
		while (addProc(p, &mem[0]) == -1)
		{
			//swap a process out
			Proc* swap = (Proc*)listRemove(inMem, 0);
			removeProc(mem, swap->id);
			swap->timesSwapped += 1;
			if (swap->timesSwapped >= 3)
				free(swap);
			else
				listAdd(procData, swap);
		}
		listAdd(inMem, p);
		avgProcs += listSize(inMem);
		avgHoles += numHoles(mem);
		memUsage += (float)memInUse(mem)/128.0f;
		//printf("Loaded ID: %c, #procs: %d, #holes: %d, %%mem: %.2f, total %%mem: %.2f\n", p->id, listSize(inMem), numHoles(mem), 100*((float)memInUse(mem)/128.0f), 100*((float)memUsage/(float)loads));
	}
	/*printf("****************\n");
	for (int a = 0; a < 128; a++)
		printf("%d : %c\n", a, mem[a]);*/
	printf("Total loads: %d, avg # procs: %.2f, avg # holes: %.2f, %% mem: %.2f\n", loads, (float)avgProcs/(float)loads, (float)avgHoles/(float)loads, 100*((float)memUsage/(float)loads));

	procData = listClear(procData, freeProc);
	inMem = listClear(inMem, freeProc);
}

int addNext(Proc* p, char* mem, int prevIndex)
{
	//printf("%c %d\n", p->id, p->size);
	for (int a = prevIndex; a < 128-p->size+1; a++)
	{
		int flag = 0;
		for (int b = a; b < a+p->size; b++)
		{
			if (mem[b] != '\0')
				flag = 1;
		}
		if (flag)
			continue;
		//add proc to memory
		for (int b = a; b < a+p->size; b++)
		{
			mem[b] = p->id;
		}
		return a;
	}
	return addProc(p, mem);
}

void nextFit(char* fileName)
{
	char mem[128];
	List* procData = loadProc(fileName);
	List* inMem = init();

	int loads = 0;
	int avgProcs = 0;
	int avgHoles = 0;
	float memUsage = 0;

	for (int a = 0; a < 128; a++)
		mem[a] = '\0';
	int index = 0;
	while (listSize(procData) > 0)
	{
		loads++;
		Proc* p = (Proc*)listRemove(procData, 0);
		int tmpIndex = index;
		while ((tmpIndex = addNext(p, &mem[0], index)) == -1)
		{
			//swap a process out
			Proc* swap = (Proc*)listRemove(inMem, 0);
			removeProc(mem, swap->id);
			swap->timesSwapped += 1;
			if (swap->timesSwapped >= 3)
				free(swap);
			else
				listAdd(procData, swap);
		}

		index = tmpIndex+p->size;
		listAdd(inMem, p);
		avgProcs += listSize(inMem);
		avgHoles += numHoles(mem);
		memUsage += (float)memInUse(mem)/128.0f;
		//printf("Loaded ID: %c, #procs: %d, #holes: %d, %%mem: %.2f, total %%mem: %.2f\n", p->id, listSize(inMem), numHoles(mem), 100*((float)memInUse(mem)/128.0f), 100*((float)memUsage/(float)loads));
	}
	
	printf("Total loads: %d, avg # procs: %.2f, avg # holes: %.2f, %% mem: %.2f\n", loads, (float)avgProcs/(float)loads, (float)avgHoles/(float)loads, 100*((float)memUsage/(float)loads));

	procData = listClear(procData, freeProc);
	inMem = listClear(inMem, freeProc);
}

int addBest(Proc* p, char* mem)
{
	int index = 0;
	int holeSize = 9999;
	//printf("%c %d\n", p->id, p->size);
	int flag = 1;
	int startIndex = 0;
	for (int a = 0; a < 128; a++)
	{
		if (flag)
		{
			if (mem[a] == '\0')
			{
				startIndex = a;
				flag = 0;
			}
		}
		else if (mem[a] != '\0')
		{
			flag = 1;
			int size = a-startIndex;
			if (size >= p->size && size < holeSize)
			{
				index = startIndex;
				holeSize = size;
			}
		}
	}
	if (flag == 0)
	{
		int size = 128-startIndex;
		if (size >= p->size && size < holeSize)
		{
			index = startIndex;
			holeSize = size;
		}
	}
	//printf("index: %d hole: %d\n", index, holeSize);
	if (holeSize != 9999)
	{
		for (int a = index; a < index+p->size; a++)
		{
			mem[a] = p->id;
		}
		return index;
	}
	return -1;
}

void bestFit(char* fileName)
{
	char mem[128];
	List* procData = loadProc(fileName);
	List* inMem = init();

	int loads = 0;
	int avgProcs = 0;
	int avgHoles = 0;
	float memUsage = 0;


	for (int a = 0; a < 128; a++)
		mem[a] = '\0';
	while (listSize(procData) > 0)
	{
		loads++;
		Proc* p = (Proc*)listRemove(procData, 0);
		while (addBest(p, &mem[0]) == -1)
		{
			//swap a process out
			Proc* swap = (Proc*)listRemove(inMem, 0);
			removeProc(mem, swap->id);
			swap->timesSwapped += 1;
			if (swap->timesSwapped >= 3)
				free(swap);
			else
				listAdd(procData, swap);
		}
		listAdd(inMem, p);
		avgProcs += listSize(inMem);
		avgHoles += numHoles(mem);
		memUsage += (float)memInUse(mem)/128.0f;
		//printf("Loaded ID: %c, #procs: %d, #holes: %d, %%mem: %.2f, total %%mem: %.2f\n", p->id, listSize(inMem), numHoles(mem), 100*((float)memInUse(mem)/128.0f), 100*((float)memUsage/(float)loads));
	}
	/*printf("****************\n");
	for (int a = 0; a < 128; a++)
		printf("%d : %c\n", a, mem[a]);*/
	printf("Total loads: %d, avg # procs: %.2f, avg # holes: %.2f, %% mem: %.2f\n", loads, (float)avgProcs/(float)loads, (float)avgHoles/(float)loads, 100*((float)memUsage/(float)loads));

	procData = listClear(procData, freeProc);
	inMem = listClear(inMem, freeProc);
}

void worstFit(char* fileName)
{
char mem[128];
	List* procData = loadProc(fileName);
	List* inMem = init();

	int loads = 0;
	int avgProcs = 0;
	int avgHoles = 0;
	float memUsage = 0;


	for (int a = 0; a < 128; a++)
		mem[a] = '\0';
	while (listSize(procData) > 0)
	{
		loads++;
		Proc* p = (Proc*)listRemove(procData, 0);
		while (addProc(p, &mem[0]) == -1)
		{
			//swap a process out
			Proc* swap = (Proc*)listRemove(inMem, 0);
			removeProc(mem, swap->id);
			swap->timesSwapped += 1;
			if (swap->timesSwapped >= 3)
				free(swap);
			else
				listAdd(procData, swap);
		}
		listAdd(inMem, p);
		avgProcs += listSize(inMem);
		avgHoles += numHoles(mem);
		memUsage += (float)memInUse(mem)/128.0f;
		//printf("Loaded ID: %c, #procs: %d, #holes: %d, %%mem: %.2f, total %%mem: %.2f\n", p->id, listSize(inMem), numHoles(mem), 100*((float)memInUse(mem)/128.0f), 100*((float)memUsage/(float)loads));
	}
	/*printf("****************\n");
	for (int a = 0; a < 128; a++)
		printf("%d : %c\n", a, mem[a]);*/
	printf("Total loads: %d, avg # procs: %.2f, avg # holes: %.2f, %% mem: %.2f\n", loads, (float)avgProcs/(float)loads, (float)avgHoles/(float)loads, 100*((float)memUsage/(float)loads));

	procData = listClear(procData, freeProc);
	inMem = listClear(inMem, freeProc);
}

int main(int argc, char* argv[])
{
	printf("First fit\n");
	firstFist("data/proc.txt");
	printf("Next fit\n");
	nextFit("data/proc.txt");
	printf("Best fit\n");
	bestFit("data/proc.txt");
	printf("Worst fit\n");
	worstFit("data/proc.txt");
	return 0;
}