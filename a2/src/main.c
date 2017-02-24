#include <stdio.h>
#include "list.h"
#include "myS.h"
#include "parse.h"
#include "sim.h"

int main(int argc, char* argv[])
{
	Data* d = parseFile("data/testcase1.txt");
	fcfs(d, 0, 0);
	RR(d, 0, 0);
	return 0;
}
