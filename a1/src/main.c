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
		printf("> ");
		line[0] = '\0';
		fgets(line, 512, stdin);
		List* myList = parseLine(line);
		runCmd(myList);
		myList = listClear(myList, freeData);
	}
}