
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "list.h"

typedef struct proc
{
	char id;
	int size;
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
		listAdd(procData, p);
		procSize = 0;
	}
	/*for (int a = 0; a < listSize(procData); a++)
		printf("%c %d\n", ((Proc*)listGet(procData, a))->id, ((Proc*)listGet(procData, a))->size);*/
	return procData;
}

Proc* removeProc(char* mem, char id)
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
	Proc* p = malloc(sizeof(Proc));
	p->id = id;
	p->size = size;
	return p;
}

void addProc(Proc* p, char* mem)
{
	printf("%c %d\n", p->id, p->size);
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
		printf("index: %d\n", a);
		//add proc to memory
		for (int b = a; b < a+p->size; b++)
		{
			mem[b] = p->id;
		} 
		break;
	}
}

int main(int argc, char* argv[])
{

	char mem[128];
	List* procData = loadProc("data/proc.txt");
	*mem = 0;
	printf("*********************\n");
	for (int a = 0; a < 128; a++)
	{
		mem[a] = '\0';
		printf("%d : %c\n", a, mem[a]);
	}
		printf("*********************\n");
	while (listSize(procData) > 0)
	{
		Proc* p = (Proc*)listRemove(procData, 0);
		addProc(p, &mem[0]);
		printf("*********************\n");
		for (int a = 0; a < 128; a++)
		{
			printf("%d : %c\n", a, mem[a]);
		}
		printf("*********************\n");
		free(p);
	}


	return 0;
}