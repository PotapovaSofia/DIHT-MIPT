#include <stdio.h>
//#include <stdout.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char** argv)
{
	if(argc < 3)
	{
		printf("Not enough arguments\n");
		exit(1);
	}
	int slave_cnt = argc -1;
	int pipe_cnt = slave_cnt -1;

	int fds[pipe_cnt][2];
	int file_rd = fileno(stdin);
	int file_wr = fileno(stdout);
	pid_t* child = malloc(slave_cnt * sizeof(pid_t));

	int i;
	for(i = 0; i < pipe_cnt; ++i)
		pipe(fds[i]);
	for(i = 0; i < slave_cnt; ++i)
	{
		if((child[i] = fork()) == 0)
		{
			if(i < pipe_cnt)
			{
				dup2(fds[i][1], file_wr);
				close(fds[i][1]);
			}
			if(i > 0)
                        {
				dup2(fds[i-1][0], file_rd);
				close(fds[i-1][0]);
                        }
			execl("/bin/sh", "sh", "-c", argv[i+1], NULL);

		}
		close(fds[i][1]);
	}

	int status;
	for(i = 0; i < slave_cnt;++i)
		waitpid(child[i], &status, 0);

	return 0;
}

