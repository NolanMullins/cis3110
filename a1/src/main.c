#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "list.h"
#include "fnc.h"

void clr(void* data)
{
	free((char*)data);
}
void clri(void* data)
{
	free((int*)data);
}

int main()
{
	char line[513];
	List* children = init();
	while (1)
	{
		printf("> ");
		line[0] = '\0';
		fgets(line, 512, stdin);
		List* myList = parseLine(line);
		runCmd(myList, children);
		myList = listClear(myList, clr);
	}
	listClear(children, clri);
}