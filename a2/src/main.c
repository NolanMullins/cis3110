#include <stdio.h>
#include "list.h"
#include "myS.h"
#include "parse.h"
#include "sim.h"

int main(int argc, char* argv[])
{
	Data* d = parseFile("data/ex.txt");
	//fcfs(d, 1, 0);
	RR(d, 1, 0);
	return 0;
}
