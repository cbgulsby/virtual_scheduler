<!-- This has got to be one of the most convoluted things I have ever made -->

Running the scheduler:

In command line, run 'make clean' first.
	- this will reset the project
Then run 'make generator'
	- this builds my processes
	- currently set to build 10 processes
Then run 'make scheduler'
	- this runs the scheduler with 10 processes
	- then it prints out the average time each process took
	  and the std dev by priority

What is contained in project:

This folder contains the initial working version of the scheduler (exec.c), 
10 process file made by the generator, a readme, a garbage file (sched.c),
a template for me to reference for my processes, and a utils file.

Stats:
	version 0:
	avg of priority 0: 39032
	avg of priority 1: 39326
	avg of priority 2: 41603
	avg of priority 3: 37322
	std dev of priority 0: 135.344006
	std dev of priority 1: 52.172790
	std dev of priority 2: 263.416021
	std dev of priority 3: 135.344006

	version 1:
	avg of priority 0: 38096
	avg of priority 1: 40932
	avg of priority 2: 41490
	avg of priority 3: 35485
	std dev of priority 0: 114.600442
	std dev of priority 1: 48.849432
	std dev of priority 2: 240.681902
	std dev of priority 3: 148.332078

	version 2:
	avg of priority 0: 40876
	avg of priority 1: 49072
	avg of priority 2: 37856
	avg of priority 3: 38564
	std dev of priority 0: 117.654098
	std dev of priority 1: 56.152390
	std dev of priority 2: 270.943890
	std dev of priority 3: 140.345090

	version 3:
	avg of priority 0: 32098
	avg of priority 1: 35762
	avg of priority 2: 33980
	avg of priority 3: 39065
	std dev of priority 0: 127.654096
	std dev of priority 1: 78.849530
	std dev of priority 2: 202.422021
	std dev of priority 3: 112.344322