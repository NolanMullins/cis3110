#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include "list.h"
#include "art.h"

int* gen(pid_t i)
{
	int* new = malloc(sizeof(int));
	*new = i;
	return new;
}

int cmp(void* data)
{
	return strcmp(">", (char*)data);
}
int cmpIn(void* data)
{
	return strcmp("<", (char*)data);
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

void tmp()
{
	while(1==1)
	{
		printf("HELLO WORLD\n");
		sleep(10);
	}
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

void arg(char** arr, int s)
{
	printf("argc = %d, args = ", s);
	int a = 0;
	while (arr[a] != NULL)
	{
		printf("%s", arr[a++]);
		if (arr[a] != NULL)
			printf(", ");
	}
	printf("\n");
}

void child(List* cmd, int fileOut, int fileIn)
{
	FILE *fOut, *fIn;
	initArt();
	if (fileOut >= 0)
	{
		free((char*)listRemove(cmd, fileOut));
		char* name = (char*)listRemove(cmd, fileOut);
		fOut = freopen(name, "w", stdout);
	}
	if (fileIn >= 0)
	{
		free((char*)listRemove(cmd, fileIn));
		char* name = (char*)listRemove(cmd, fileIn);
		fIn = freopen(name, "r", stdin);
	}

	int size = listSize(cmd);
	char** args = conv(cmd);
	if (strcmp("add", args[0]) == 0)
		add(&args[1]);
	else if (strcmp("args", args[0]) == 0)
		arg(&args[1], size-1);
	else if (strcmp("tmp", args[0]) == 0)
		tmp();
	else if (strcmp("art", args[0]) == 0)
		randomArt();
	else
		execvp(args[0], args);
	free(args[size-1]);
	free(args);
	if (fileOut > 0)
		fclose(fOut);
	if (fileIn > 0)
		fclose(fIn);
	exit(0);
}

int runCmd(List* cmd, List* children)
{
	if (strcmp("exit", (char*)listGet(cmd, 0)) == 0)
		return 1;
	//check to see if it should be run in the background
	char* last = (char*)listGet(cmd, listSize(cmd)-1);
	int bgrd = strcmp("&", last);
	int fileOut = indexOf(cmd, cmp);
	int fileIn = indexOf(cmd, cmpIn);
	if (bgrd == 0)
		free((char*)listRemove(cmd, listSize(cmd)-1));
	//fork to run process on
	pid_t id = fork();

	if (id == 0)
		child(cmd, fileOut, fileIn);
	else if (id > 0 && bgrd == 0)
	{
		//kill(id, SIGKILL);
		listAdd(children, gen(id));//gen(&id));
	}
	else if (id > 0)
	{
		int status;
		waitpid(id, &status, 0);
		if (status != 0)
			printf("Child ended with error\n");
	}
	return 0;
}

