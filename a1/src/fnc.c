#include <stdio.h>
#include <string.h>
#include "list.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void clr(void* data)
{
	free((char*)data);
}

char** conv(List* cmd)
{
	char** arr = malloc(sizeof(char*)*(listSize(cmd)+1));
	int a = 0;
	while (listSize(cmd) > 0)
		arr[a++] = (char*)listRemove(cmd, 0);
	return arr;
}

void child(List* cmd)
{
	//int size = listSize(cmd);
	char** args = conv(cmd);

	execvp(args[0], args);

	free(args);
	exit(0);
}

void runCmd(List* cmd)
{
	if (strcmp("exit", (char*)listGet(cmd, 0)) == 0)
		exit(0);
	//fork to run process on
	pid_t id = fork();
	if (id == 0)
		child(cmd);
	else if (id > 0)
	{
		int status;
		waitpid(id, &status, 0);
		if (status != 0)
			printf("Child ended with error\n");
	}
}

