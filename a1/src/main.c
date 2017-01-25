#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	sigset(1);
	printf("Hi\n");
}