#include <stdio.h>
#include "list.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void child(List* cmd)
{
	printf("%s\n", (char*)listGet(cmd,0));
	exit(0);
}

void runCmd(List* cmd)
{
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

