
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

void removeProc(char* mem, char id)
{
	int size = 0;
	for (int a = 0; a < 128; a++)
	{
		if (mem[a] == id)
		{
			mem[a] = '\0';
			size++;
		}
	}
}

int addProc(Proc* p, char* mem)
{
	//printf("%c %d\n", p->id, p->size);
	for (int a = 0; a < 128-p->size; a++)
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

int main(int argc, char* argv[])
{
	char mem[128];
	List* procData = loadProc("data/proc.txt");
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
		printf("Loaded ID: %c, #procs: %d, #holes: %d, %%mem: %.2f, total %%mem: %.2f\n", p->id, listSize(inMem), numHoles(mem), 100*((float)memInUse(mem)/128.0f), 100*((float)memUsage/(float)loads));
	}

	//for (int a = 0; a < 128; a++)
	//	printf("%d : %c\n", a, mem[a]);
	printf("Total loads: %d, avg # procs: %.2f, avg # holes: %.2f, %% mem: %.2f\n", loads, (float)avgProcs/(float)loads, (float)avgHoles/(float)loads, 100*((float)memUsage/(float)loads));

	procData = listClear(procData, freeProc);
	inMem = listClear(inMem, freeProc);

	return 0;
}