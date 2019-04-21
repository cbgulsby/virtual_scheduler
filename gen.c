#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// gen

void
proc_gen(int n)
{
	FILE* 	fp;
	char	s[32];


	sprintf(s, "proc%i", n);
	fp = fopen(s, "w");
	if (!fp) dam("file did not open");

	for(int i = 0; i < 256; ++i)
	{

	}

	fprintf(fp, "%9i\n", 0);								// position
	fprintf(fp, "%9i\n", random_between(0,3));				// priority
	fprintf(fp, "%9i\n", random_between(10,200));			// memory
	fprintf(fp, "%9i\n", 5000);								// runtime
	fprintf(fp, "%9i\n", 0);								// start time
	fprintf(fp, "%9i\n", 0);								// end time

	int i;
	for (i = 0; i < 100; i++)
	{
		fprintf(fp, "%3i\n", i);
	}
}

int 
main()
{
	int i;
	for (i = 0; i < 10; i++)
	{	
		proc_gen(i);
	}
	/*for (i = 0; i < fileNum; i++) 					//number of files
	{
		fp = fopen("mem", "w");
		fprintf(fp, "000 %9i", w);
		fprintf(fp, "001 %9i", x);
		fprintf(fp, "010 %9i", y);
		fprintf(fp, "011 %9i", z);
		for (j = 0; j < numData; j++)				//numbers in the file
		{
			fprintf(fp, "");
		}
		fclose();
	}*/
	return 0;
}