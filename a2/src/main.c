#include <stdio.h>
#include "list.h"
#include "myS.h"
#include "parse.h"


int main(int argc, char* argv[])
{
	Data* d = parseFile("data/ex.txt");
	List* procs = d->procs;
	for (int a = 0; a < listSize(procs); a++)
	{
		printf("Process: %d\n", a);
		Proc* p = (Proc*)listGet(procs, a);
		List* threads = p->threads;
		for (int b = 0; b < listSize(threads); b++)
		{
			Thread* t = (Thread*)listGet(threads, b);
			printf("ThreadNum: %d, %d\n", t->threadNum, t->arrival);
		}
	}
	return 0;
}