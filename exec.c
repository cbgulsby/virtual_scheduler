#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

static FILE* 	fp;
static int		pos, remain;
int qs[8][1 << 20];
int ts[8] = {0,0,0,0,0,0,0,0};
int x = 4096;
int mem_alloc[4096];
int mem_alloc_ptr = 0;
int disk = 8192;
typedef struct {
	long mem: 12;
	long disk: 13;
	long dirty: 1;
} pgTableEntry;

typedef struct 
{
	pgTableEntry pgTable[256];
}pgTable;

pgTable page;

void
proc_open (int n)
{
	char	s[32];
	sprintf(s, "proc%i", n);
	fp = fopen(s, "r+");
	if (!fp) dam("file did not open");
	fseek(fp, 0, 0);
	fscanf(fp, "%i", &pos);
	fseek(fp, 30, 0);
	fscanf(fp, "%i", &remain);
}

//closes process file
void
proc_close ()
{
	fseek(fp, 0, 0);
	fprintf(fp, "%9i\n", pos);
	fseek(fp, 30, 0);
	fprintf(fp, "%9i", remain);
	fclose(fp);
}

//returns time process took to execute
int
proc_exec (int n, int quant)
{
	int count = 0;
	int x;

	proc_open(n);
	fseek(fp, 60 + (pos << 2), 0);
	for (;;)
	{
		if (fscanf(fp, "%i", &x) == EOF)
		{
			// printf("eof\n");
			remain -= count;
			count |= 1 << 29;
			break;
		}
		count += x;
		++pos;
		// printf("count : %d\n", count);
		if (count > remain)
		{
			// printf("time alloted\n");
			remain -= count;
			count |= 1 << 28;
			break;
		}
		if (count > quant)
		{
			// printf("quant reached\n");
			remain -= count;
			count |= 1 << 30;
			break;
		}
		// i = random_between(0,100);
		// if (i > 2)
		// {
		// 	printf("here\n");
		// 	remain -= count;
		// 	break;
		// }
	}
	proc_close();
	return count;
}

//gets priority of process (needs to be changed to accomodate page table)
int 
get_priority (int proc)
{
	proc_open(proc);
	int priority;
	fseek(fp, 10, 0);
	fscanf(fp, "%i", &priority);
	proc_close();
	return priority;
}

//gets memory of process (needs to be changed to accomodate page table)
int
get_memory (int proc)
{
	proc_open(proc);
	int memory;
	fseek(fp, 20, 0);
	fscanf(fp, "%i", &memory);
	proc_close();
	return memory;
}

void
set_start(int proc, int local_time)
{
	proc_open(proc);
	fseek(fp, 40, 0);
	fprintf(fp, "%9i\n", local_time);
	proc_close();
}

void
set_exit (int proc, int local_time)
{
	proc_open(proc);
	fseek(fp, 40, 0);
	int entry_time = fscanf(fp, "%9i", &entry_time);
	fseek(fp, 50, 0);
	fprintf(fp, "%9i\n", local_time + entry_time);
	proc_close();
}

// dont know how it works
void
enqueue (int priority, int process)
{
	qs[priority][ts[priority]++] = process;
}

// dont know how it works
void 
dequeue (int priority, int spot)
{
	qs[priority][spot] = qs[priority][--ts[priority]]; 
}

void
sched()
{
	int i, j, x, time = 0, tempP, random_issue_time = random_between(400,40000);
	// int random_issue_time = random_between(20000,10000000);
	for (;;)
	{	
		for (i = 0; i < 4; i++)
		{
			int bq;
			for (bq = 0; bq < ts[4]; ++bq)
			{
				if (qs[5][bq] <= time)
				{
					// printf("Unblocking %i\n", qs[4][bq]);
					tempP = get_priority(qs[4][bq]);
					dequeue(4,bq);
					dequeue(5,bq);
					bq--;
				}
			}
			if (random_issue_time <= time && ts[6] != 0) // adding a process to the queue
			{
				tempP = get_priority(qs[6][0]);
				set_start(qs[6][0], time);
				enqueue(tempP, qs[6][0]);
				dequeue(6,0);
				random_issue_time = 100;
			}

			for (j = 0; j < ts[i]; ++j)
			{
				if (ts[i] == 0)
				{
					break;
				}
				x = proc_exec(qs[i][j],1000);
				if (x & (1 << 30))
				{
					// printf("quantum reached: %i, %i nothing happens\n", i, qs[i][j]);
				}
				else if (x & (1 << 29))
				{
					// printf("time reached: %i, %i process deleted\n", i, qs[i][j]);
					// this is where you set end time for process
					// printf("about to exit\n");
					// printf("time is: %d\n", time);
					set_exit(qs[i][j],time);
						// proc_open(qs[i][j]);
						// fseek(fp, 50, 0);
						// int end_time = fscanf(fp, "%i", &end_time);
						// fseek(fp, 40, 0);
						// int start_time = fscanf(fp, "%i", &start_time);
						// int temp = end_time - start_time;
						// qs[7][qs[i][j]] += temp;
						// proc_close(qs[i][j]);
					// stats();
					enqueue(7,qs[i][j]);
					dequeue(i,j);
					j--;
				}
				else if (x & (1 << 28))
				{
					// printf("process finished\n");
					// this is where you set end time for process
					// printf("about to exit\n");
					// printf("time is: %d\n", time);
					set_exit(qs[i][j],time);
						// proc_open(qs[i][j]);
						// fseek(fp, 50, 0);
						// int end_time = fscanf(fp, "%i", &end_time);
						// fseek(fp, 40, 0);
						// int start_time = fscanf(fp, "%i", &start_time);
						// int temp = end_time - start_time;
						// qs[7][qs[i][j]] += temp;
						// proc_close(qs[i][j]);
					// stats();
					enqueue(7,qs[i][j]);
					dequeue(i,j);
					j--;
				}
				else
				{
					// printf("blocked\n");
					int random_time = random_between(4,100);
					enqueue(4,qs[i][j]);
					enqueue(5,random_time + time); // 100 will change to random time
					dequeue(i,j);
					j--;
				}

				x &= 0x0ffffff;
				time += x;
			}
		}

		if (ts[0] == 0 && ts[1] == 0 && ts[2] == 0 && ts[3] == 0 && ts[4] != 0 && ts[6] == 0)
		{
			// printf("Everything is blocked\n");
			time = qs[5][0];
		}
		else if (ts[0] == 0 && ts[1] == 0 && ts[2] == 0 && ts[3] == 0 && ts[4] == 0 && ts[6] != 0)
		{
			// printf("Something is left waiting in the initial arr\n");
			time = random_issue_time;
		}
		else if (ts[0] == 0 && ts[1] == 0 && ts[2] == 0 && ts[3] == 0 && ts[4] != 0 && ts[6] != 0)
		{
			// printf("No process is ready\n");
			if (random_issue_time < qs[5][0])
			{
				time = qs[5][0];
			}
			else
			{
				time = random_issue_time;
			}
		}
		else if (ts[0] == 0 && ts[1] == 0 && ts[2] == 0 && ts[3] == 0 && ts[4] == 0 && ts[6] == 0)
		{
			// printf("Everything is done\n");
			break;
		}
	}
}

void
load_proc (int num_proc)
{
	int i;
	for (i = 0; i < num_proc; ++i)
	{
		enqueue(6,i);
	}
	sched();
}

int get_end(int spot) {
  proc_open(spot);
  fseek(fp, 50, 0);
	int end_time;
	fscanf(fp, "%i", &end_time);
	//printf("end_time: %d\n", end_time);
	fseek(fp, 40, 0);
	int start_time;
	fscanf(fp, "%i", &start_time);
	int temp = end_time - start_time;
	//printf("time gotten: %d\n", temp);
  proc_close();
  return temp;
}

void
stats()
{
	int sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	int avg0, avg1, avg2, avg3;
	int total0 = 0, total1 = 0, total2 = 0, total3 = 0, i;
	double std_dev0 = 0, std_dev1 = 0, std_dev2 = 0, std_dev3 = 0;
	int temp_dev0 = 0, temp_dev1 = 0, temp_dev2 = 0, temp_dev3 = 0;
	int total_dev0 = 0, total_dev1 = 0, total_dev2 = 0, total_dev3 = 0;
	for (i = 0; i < ts[7]; ++i)
	{
		// printf("end is: %i\n", get_end(qs[7][i]));
		if (get_priority(qs[7][i]) == 0) 
		{
			sum0 += get_end(qs[7][i]);
			total0++;
		}
		if (get_priority(qs[7][i]) == 1) 
		{
			sum1 += get_end(qs[7][i]);
			total1++;
		}
		if (get_priority(qs[7][i]) == 2) 
		{
			sum2 += get_end(qs[7][i]);
			total2++;
		}
		if (get_priority(qs[7][i]) == 3) 
		{
			sum3 += get_end(qs[7][i]);
			total3++;
		}
		//sum += get_end(qs[7][i]);
	}
	avg0 = sum0 / total0;
	printf("avg of priority 0: %d\n", avg0);
	avg1 = sum1 / total1;
	printf("avg of priority 1: %d\n", avg1);
	avg2 = sum2 / total2;
	printf("avg of priority 2: %d\n", avg2);
	avg3 = sum3 / total3;
	printf("avg of priority 3: %d\n", avg3);
	for (i = 0; i < ts[7]; ++i)
	{
		if (get_priority(qs[7][i]) == 0) 
		{
			temp_dev0 = get_end(qs[7][i]) - avg0;
			total_dev0 += temp_dev0;
		}
		if (get_priority(qs[7][i]) == 1) 
		{
			temp_dev1 = get_end(qs[7][i]) - avg1;
			total_dev1 += temp_dev1;
		}
		if (get_priority(qs[7][i]) == 2) 
		{
			temp_dev2 = get_end(qs[7][i]) - avg2;
			total_dev2 += temp_dev2;
		}
		if (get_priority(qs[7][i]) == 3) 
		{
			temp_dev3 = get_end(qs[7][i]) - avg3;
			total_dev3 += temp_dev3;
		}
	}
	temp_dev0 = temp_dev0 * temp_dev0;
	temp_dev0 = temp_dev0 / ts[7];
	std_dev0 = sqrt(temp_dev0);
	printf("std dev of priority 0: %f\n", std_dev0);
	temp_dev1 = temp_dev1 * temp_dev1;
	temp_dev1 = temp_dev1 / ts[7];
	std_dev1 = sqrt(temp_dev1);
	printf("std dev of priority 1: %f\n", std_dev1);
	temp_dev2 = temp_dev2 * temp_dev2;
	temp_dev2 = temp_dev2 / ts[7];
	std_dev2 = sqrt(temp_dev2);
	printf("std dev of priority 2: %f\n", std_dev2);
	temp_dev3 = temp_dev3 * temp_dev3;
	temp_dev3 = temp_dev3 / ts[7];
	std_dev3 = sqrt(temp_dev3);
	printf("std dev of priority 3: %f\n", std_dev3);
}

//functions added for pageTable: 
/*
void getPageTable(): reads pageTable from file (every time proc opens)
void putPageTable(): prints pageTable to file (every time proc closes)
int alloc_mem()
{
	if(x) return x--;
	if(mem_alloc_ptr == 0) return 0;
	else return mem_alloc[mem_alloc_ptr--];
}
void free_mem(int pageIndex)
{
	mem_alloc[mem_alloc_ptr++] = pageIndex;
}
int get_disk():
void free_disk():
int LRU():
*/
//opens process file

void getPageTable()
{
	int tempMem = 0;
	int tempDisk = 0;
	int tempDirty = 0;
	for(int i = 0; i < 256; i++)
	{
		fseek(fp, 0, 0);
		fscanf(fp, "%6i", &tempMem);
		fseek(fp, 6, 0);
		fscanf(fp, "%7i", &tempDisk);
		fseek(fp, 7, 0);
		fscanf(fp, "%i", &tempDirty);
		page.pgTable[i].mem = tempMem;
		page.pgTable[i].disk = tempDisk;
		page.pgTable[i].dirty = tempDirty;
	}
}

int 
main ()
{
	load_proc(10);
  	printf("I got so far\nBut in the end\nIt didn't even matter\n");

  	//stats();
  	
	getPageTable();
	printf("mem is: %i\n", page.pgTable[0].mem);

	return 0;
}
