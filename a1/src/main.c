#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "util.h"
#include "list.h"

void freeData(void* data)
{
	free((char*)data);
}

int main()
{
	char line[513];
	while (1)
	{
		printf("$ ");
		line[0] = '\0';
		fgets(line, 512, stdin);
		List* myList = parseLine(line);
		for (int a = 0; a < listSize(myList); a++)
			printf("%s\n", (char*)listGet(myList, a));
		if (strcmp("exit", (char*)listGet(myList, 0)) == 0)
		{
			listClear(myList, freeData);
			exit(0);
		}
		listClear(myList, freeData);
	}
}