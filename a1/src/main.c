#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
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
	int status = 0;
	while (status == 0)
	{
		printf("> ");
		line[0] = '\0';
		fgets(line, 512, stdin);
		List* myList = parseLine(line);
		status = runCmd(myList, children);
		myList = listClear(myList, clr);
	}

	int a;
	for (a = 0; a < listSize(children); a++)
		kill(*(int*)listGet(children, a), SIGKILL);
	listClear(children, clri);
	return 0;
}