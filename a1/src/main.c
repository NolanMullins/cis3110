#include <stdio.h>
#include <sys/types.h>

int main()
{
	printf("%d\n", fork());
	printf("Hi\n");
}