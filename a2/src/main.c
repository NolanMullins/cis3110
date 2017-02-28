#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "myS.h"
#include "parse.h"
#include "sim.h"

//used to pass into list function when it needs to del its data
void del(void* data)
{
	free((Thread*)data);
}

int main(int argc, char* argv[])
{
	int detail = 0, verbose = 0, rr = 0, q = 100;
	//look for flags
	for (int a = 0; a < argc; a++)
	{
		if (strcmp("-d", argv[a]) == 0)
			detail = 1;
		else if (strcmp("-v", argv[a]) == 0)
			verbose = 1;
		//if round robin, get quantum
		else if (strcmp("-r", argv[a]) == 0)
		{
			rr = 1;
			if (a < argc+1)
			{
				q = atoi(argv[a+1]);
				if (q <= 0)
					q = 100;
			}
		}
	}
	//check if a file was passed in
	if (isatty(STDIN_FILENO))
  	{
   		printf("No file passed\n");
    	exit(1);
    }
    //parse into data structure
	Data* d = parseFile("data/testcase1.txt");
	if (rr == 0)
		fcfs(d, detail, verbose);
	else
		RR(d, detail, verbose, q);
	//memory cleaning
	d->threads = listClear(d->threads, del);
	free(d);
	return 0;
}
