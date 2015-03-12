#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>
#include <time.h>


#define REAL_BIG_VALUE 1000000
#define CHILDS_CNT 4

pid_t child[CHILDS_CNT];

void abort_prg(const char* error)
{
	perror(error);
	exit(1);
}

void child_proc(int fds[2])
{
	srand(time(NULL)^getpid()<<16);	
	close(fds[0]);	
	int ret = 0;
	double p[2]; //point
	int i, j;
	for(j = 0;j < REAL_BIG_VALUE; ++j)
	{	
		for(i = 0; i < 2; ++i)
			p[i] = ((rand()/(double)2147483646)*6);
		if(((p[0]*p[0] + p[1]*p[1] - 1)*(p[0]*p[0] + p[1]*p[1] - 1)*(p[0]*p[0] + p[1]*p[1] - 1) - p[0]*p[0]*p[1]*p[1]*p[1])<0)
			ret += 1;
	}
	if(sizeof(int) != write(fds[1], &ret, sizeof(int)))
		abort_prg("Write error\n");	
	close(fds[1]);
}

int get_result(int fd)
{
	int res;
	if(sizeof(int) != read(fd, &res, sizeof(int)))
		abort_prg("Read_error");
	return res;
}

int parent_proc(int fds[CHILDS_CNT][2])
{
	int i;
	for(i = 0; i < CHILDS_CNT; ++i)
		close(fds[i][1]);
	int sum = 0;
	for(i = 0; i < CHILDS_CNT; ++i)
	{
		sum+=get_result(fds[i][0]);
	}
	for(i = 0; i < CHILDS_CNT; ++i)
                close(fds[i][0]);
	int status;
	for(i = 0; i < CHILDS_CNT; ++i)
	{
		if(child[i] != waitpid(child[i], &status, 0))
			abort_prg("Waitpid error");
		if(0 != status)
			fprintf(stderr, "Child has some errors: %d\n", status);
	}

	return sum;	
}


int main(int argc, char* argv[])
{
	//create pipes
	int fds[CHILDS_CNT][2];
	int i;
	for(i = 0; i < CHILDS_CNT; ++i)
	{
		if(0 != pipe(fds[i]))
			 abort_prg("can't create pipe");
	}

	//working with childs
	for(i = 0; i < CHILDS_CNT; ++i)
	{
		if((child[i] = fork()) < 0)
			abort_prg("Can't create child\n");
		if(0 == child[i])
		{
			child_proc(fds[i]);	
			return 0;
		}
	}
	
	int cnt = parent_proc(fds); 				//count of points under
	double square = ((double)cnt/REAL_BIG_VALUE)*16;	
	//S = S_par*cnt/n; 
	printf("Square = %lf\n", square);

	return 0;
}
