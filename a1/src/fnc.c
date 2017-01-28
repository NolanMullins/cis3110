#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "list.h"

int* gen(int i)
{
	int* new = malloc(sizeof(int));
	*new = i;
	return new;
}

char** conv(List* cmd)
{
	char** arr = malloc(sizeof(char*)*(listSize(cmd)+1));
	int a = 0;
	while (listSize(cmd) > 0)
		arr[a++] = (char*)listRemove(cmd, 0);
	arr[a] = malloc(sizeof(char));
	arr[a] = '\0';
	return arr;
}

void add(char** arr)
{
	int a = 0;
	int sum = 0;
	while (arr[a] != NULL)
	{
		if (arr[a][1] == 'x')
			sum += (int)strtol(arr[a], NULL, 0);
		else
			sum += atoi(arr[a]);
		printf("%s ", arr[a++]);
		if (arr[a] != NULL)
			printf("+ ");
	}
	printf("= %d\n", sum);
}

void child(List* cmd)
{
	int size = listSize(cmd);
	char** args = conv(cmd);
	if (strcmp("add", args[0]) == 0)
		add(&args[1]);
	else
		execvp(args[0], args);
	free(args[size-1]);
	free(args);
	exit(0);
}

void runCmd(List* cmd, List* children)
{
	if (strcmp("exit", (char*)listGet(cmd, 0)) == 0)
		exit(0);
	//check to see if it should be run in the background
	char* last = (char*)listGet(cmd, listSize(cmd)-1);
	int bgrd = strcmp("&", last);
	int file = 1;
	if (listSize(cmd) > 2)
		file = strcmp(">", (char*)listGet(cmd, listSize(cmd)-2));
	if (bgrd == 0)
		free((char*)listRemove(cmd, listSize(cmd)-1));
	//fork to run process on
	pid_t id = fork();

	if (id == 0)
		child(cmd);
	else if (id > 0 && bgrd == 0)
	{
		listAdd(children, gen((int)id));
	}
	else if (id > 0)
	{
		int status;
		waitpid(id, &status, 0);
		if (status != 0)
			printf("Child ended with error\n");
	}
}

