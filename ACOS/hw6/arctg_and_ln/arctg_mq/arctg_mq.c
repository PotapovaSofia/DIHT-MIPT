#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>

#define ITER_COUNT 5
#define abs(x) ((x) >=0 ? (x) :(-x))
#define PI 3.14159265358
#define DEGREES_PER_RADIAN 57.296
#define QUEUE_NAME "/mq-ln"


mqd_t mqd;
pid_t child[2];

void abort_prg(const char* error)
{
        perror(error);
	mq_unlink(QUEUE_NAME);
        exit(1);
}

void init_queue()
{
	struct mq_attr MQ;
	MQ.mq_maxmsg = 10;
	MQ.mq_msgsize = 1024;
	mqd = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &MQ);
	if(mqd == (mqd_t)-1)
		abort_prg("Can't open\n");
	mq_close(mqd);	
}

void send_msg(double msg, int id)
{
	mqd_t mqdS = mq_open(QUEUE_NAME, O_WRONLY);
	if(mqdS == (mqd_t)-1)
		abort_prg("Child can't open");
	char buf[1024];
	sprintf(buf, "%lf", msg);
	if(0 != mq_send(mqdS, buf, sizeof(buf), id))
		abort_prg("Child can't send");
	mq_close(mqdS);

}

void child_proc(double x, int id, int fds[2]) {
	double t = (0 == id) ? x : -x*x*x/3;
	
	send_msg(t, id); 

	int i;
        for (i = 1; i < ITER_COUNT; ++i)
	{
		if(0 == id)
			t*=x*x*x*x*(4*i-3)/(4*i+1);
		else
			t*=x*x*x*x*(4*i-1)/(4*i+3);
		send_msg(t, id);
	}
}

double receive_msg()
{
	mqd_t mqdR = mq_open(QUEUE_NAME, O_RDONLY);
	if(mqdR == (mqd_t)-1)
		abort_prg("Parent can't open\n");
	char buf[2048];
	if(0 != mq_receive(mqdR, buf, sizeof(buf), NULL));
	mq_close(mqdR);
	return atof(buf);
}

double parent_proc() {
	double sum = 0.0;
	double temp;
	int id;

	int i;
	for (i = 0; i < ITER_COUNT * 2; ++i)
	{
		temp = receive_msg();
		sum +=temp;
		printf("[%d]\tReceived data[%.5lf]\t\n", i, temp);
	}
	
	int status;
        for(i = 0; i < 2; ++i)
	{
		if (child[i] != waitpid(child[i], &status, 0))
                	abort_prg("waitpid error");
        	if (status != 0)
                	fprintf(stderr, "child has some errors: %d\n", status);
	}

	return sum;
}
int main(int argc, const char* argv[]) {
  	double x= 0.0;
	int sign = 1;
	double div = 0.0;	

	int fds[2][2];
        if (argc < 2)
		{	
                	printf("You forgot to enter a value\n");
			exit(0);
		}
	sscanf(argv[1], "%lf", &x);

	double old = x;
        if(x>1)
        {
                x = 1/x;
                sign = -1;
                div = PI/2;
        }

	init_queue();
	
	int i;
        for(i = 0; i < 2; ++i)
        {
                if((child[i] = fork()) < 0)
			abort_prg("can't create child\n");
		if(child[i] == 0)
                {  
			child_proc(x, i, fds[i]);
                	return 0;
        	}
        }
	
        double res = parent_proc();
	double ans = (div + sign*res)*DEGREES_PER_RADIAN;
	printf("\narctg(%lf) = %lf + %d*arctg(%lf))\n", old, div, sign, x);
        printf("arctg(%lf): %.7lf\n", old, ans);

	
	if(0 != mq_unlink(QUEUE_NAME))
                abort_prg("mq_unlink");

        return 0;
}
           
