#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "list.h"
#include "fnc.h"

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
		int tmp = strcmp("exit", (char*)listGet(myList, 0));
		if (tmp != 0)
			runCmd(myList);
		listClear(myList, freeData);
		if (tmp == 0)
			exit(0);
	}
}