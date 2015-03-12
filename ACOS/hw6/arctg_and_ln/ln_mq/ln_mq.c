#define _GNU_SOURSE
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

#define ITER_COUNT 100 
#define abs(x) ((x) >=0 ? (x) :(-x))
#define ENUMBER 2.7182818284
#define PI 3.1415926
#define QUEUE_NAME "/mq-ln"

mqd_t mqd;
pid_t child[2];

typedef struct {
	double r;
	double i;
}cmpl;

void abort_prg(const char* error) {
        perror(error);
	mq_unlink(QUEUE_NAME);
        exit(1);
}

cmpl cmpl_plus(cmpl a, cmpl b) 	{
        cmpl res;
        res.r = a.r + b.r;
        res.i = a.i + b.i;
        return res;
}

cmpl cmpl_mult(cmpl a, cmpl b)	{
	cmpl res;
	res.r = a.r * b.r - a.i * b.i;
	res.i = a.r * b.i + a.i * b.r;
	return res;
}

cmpl cmpl_div(cmpl a, cmpl b)	{
	cmpl res;
	res.r = (a.r * b.r + a.i * b.i) / (b.r * b.r  + b.i * b.i);
	res.i = (a.i * b.r - a.r * b.i) / (b.r * b.r + b.i * b.i);
	return res;
}

cmpl cmpl_deg(cmpl a, int n)	{
	cmpl res;
	res.r = 1;
	res.i = 0;
	int j;
	for(j = 0; j < n; ++j)
		res = cmpl_mult(a, res);
	return res;
}

void init_queue()	{
	struct mq_attr MQ;
	MQ.mq_maxmsg = 10;
	MQ.mq_msgsize = 1024;
	mqd = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &MQ);
	if(mqd == (mqd_t)-1)
		abort_prg("Can't open\n");
	mq_close(mqd);	
}

void send_msg(cmpl msg, int id) {
	mqd_t mqdS = mq_open(QUEUE_NAME, O_WRONLY);
	if(mqdS == (mqd_t)-1)
		abort_prg("Child can't open");
	char buf[1024];
	sprintf(buf, "%lf %lf", msg.r, msg.i);
	if(0 != mq_send(mqdS, buf, sizeof(buf), id))
		abort_prg("Child can't send");
	mq_close(mqdS);
}

void child_proc(cmpl x, int id) {
	int k = (0 == id) ? 1 : 3;	
	cmpl t, p;
	t.r = cmpl_deg(x, k).r;
	t.i = cmpl_deg(x, k).i;
	p.r = cmpl_deg(x, 4).r;
	p.i = cmpl_deg(x, 4).i;

	for(; k < ITER_COUNT; k += 4) {
		cmpl temp;
		temp.r = t.r / (double)k;
		temp.i = t.i / (double)k;
		send_msg(temp, id);
		t.r = cmpl_mult(t, p).r;
		t.i = cmpl_mult(t, p).i;
	}
}

cmpl receive_msg() {
	mqd_t mqdR = mq_open(QUEUE_NAME, O_RDONLY);
	if(mqdR == (mqd_t)-1)
		abort_prg("Parent can't open\n");
	cmpl res;
	char buf[2048];
	if(0 != mq_receive(mqdR, buf, sizeof(buf), NULL));
	sscanf(buf, "%lf %lf", &(res.r), &(res.i));
	mq_close(mqdR);
	return res;
}

cmpl parent_proc() {
	cmpl sum;
	sum.r = 0;
	sum.i = 0;
	cmpl temp, t;
	int i;
	for (i = 0; i < ITER_COUNT / 2; ++i)	{
		temp = receive_msg();
		sum.r = cmpl_plus(sum, temp).r;
		sum.i = cmpl_plus(sum, temp).i;
		printf("[%d]\tReceived data[%.5lf + i * %lf]\t\n", i, temp.r, temp.i);
	}
	
	int status;
        for(i = 0; i < 2; ++i)	{
		if (child[i] != waitpid(child[i], &status, 0))
                	abort_prg("waitpid error");
        	if (status != 0)
                	fprintf(stderr, "child has some errors: %d\n", status);
	}
	
	sum.r *=2;
	sum.i *=2;
	return sum;
}
int main(int argc, const char* argv[]) {
  	double x = 0.0;
	double y = 0.0;

        if (argc < 3) {	
               	printf("You forgot to enter a value\n");
		exit(0);
	}
	if(1 != sscanf(argv[1], "%lf", &x) || 1 != sscanf(argv[2], "%lf", &y))
		abort_prg("bad scanf");
	if(0 == x && 0 == y) {
		printf("Result:\t%lf + i * %lf\n", x, y);
		return 0;
	}
	if(0 == x && 1 == y) { 
	        printf("Result:\t%lf + i * %lf\n", x, PI/2);
		return 0;
	}

	int flag = 0;
	//ln(-x) = ln(x) + i * PI * (2k + 1)
	if(x < 0)	{
		x = x - 2*x;
		flag = 1;
	}

	//ln(x*e^(-k)) = ln(x) + ln(e^(-k));
	int k = 0, fl = 0;
	if((sqrt((x - 1)*(x - 1) + y*y)) > 1)	{
		while(sqrt((((pow(ENUMBER, k)*x) -1) * ((pow(ENUMBER, k)*x) -1)) + pow(ENUMBER,k)*pow(ENUMBER, k)*y*y) > 1)
			k--;
		x *= pow(ENUMBER, k);
		y *= pow(ENUMBER, k);
		fl = 1;
	}

	//ln((1+x)/(1-x)) = 2*(x + x^3/3 + x^5/5 + ...)
	cmpl a, b, c;
        a.r = x - 1;
        a.i = y;
        b.r = x + 1;
        b.i = y;
        c.r = cmpl_div(a, b).r;
        c.i = cmpl_div(a, b).i;

	init_queue();
	
	int i;
        for(i = 0; i < 2; ++i)        {
                if((child[i] = fork()) < 0)
			abort_prg("can't create child\n");
		if(child[i] == 0)     {  
			child_proc(c, i);
                	return 0;
        	}
        }
	
        cmpl res = parent_proc();
	
	if(fl)	 res.r += abs(k);
	if(flag) res.i = PI - res.i;
	
	printf("Result:\t%lf + i * %lf\n", res.r, res.i);

	if(0 != mq_unlink(QUEUE_NAME))
                abort_prg("mq_unlink");

        return 0;
}
           
