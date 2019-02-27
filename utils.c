#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void
dam(char* s)
{
	printf("%s\n", s);
	exit(1);
}

int
random_between(int from, int to)
{
	return rand() % (to - from + 1) + from;
}


/*
int 
main()
{
	int i;
	for(i=0; i < 1000; i++)
		printf("%d\n", random_between(0,99));

	dam("broke");
}
*/
