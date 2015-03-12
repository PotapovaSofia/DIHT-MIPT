#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define ITER_COUNT 20
#define abs(x) ((x) >=0 ? (x) :(-x))
#define PI 3.14159265358
#define DEGREES_PER_RADIAN 57.296

pid_t child[2];
volatile sig_atomic_t flag[2] = {0,0};

void abort_prg(const char* error) {
        perror(error);
        exit(1);
}

void signal_handler0(int signo) {
	flag[0]=1;
}

void signal_handler1(int signo) {
        flag[1]=1;
}
 
void setup_handler() {
	if(SIG_ERR == signal(SIGUSR1, signal_handler0))
		abort_prg("signal SIGUSR1");
	if(SIG_ERR == signal(SIGUSR2, signal_handler1))
                abort_prg("signal SIGUSR2");
}	

void wait_parent(int id) {
	printf("[%u]i'm waiting\n", getpid());
        if(id == 0) {
                while (0 == flag[0])
                        ;
                flag[0] = 0;
        } else {
                while(0 == flag[1])
                        ;
                flag[1] = 0;
        }	
}

void child_proc(double x, int id, int fds[2]) {
        close(fds[0]);
	int signal_id = (0 == id) ? SIGUSR1 : SIGUSR2;
	double t = (0 == id) ? x : -x*x*x/3;
	if (sizeof(double) != write(fds[1], &t, sizeof(double)))
                abort_prg("write error");
	kill(getppid(), signal_id); //сигнал SIGUSR посылается родительскому процессу
	printf("Signal fom chiild[%u] to parent\n", getpid());

	int i;
        for (i = 1; i < ITER_COUNT; ++i) {
		wait_parent(id);        //ожидание отклика от родительского процесса 
		if(0 == id)
			t*=x*x*x*x*(4*i-3)/(4*i+1);
		else
			t*=x*x*x*x*(4*i-1)/(4*i+3);
		
        	if (sizeof(double) != write(fds[1], &t, sizeof(double)))
                	abort_prg("write error");
		kill(getppid(), signal_id); //сигнал SIGUSR посылается родительскому процессу
		printf("Signal fom child[%u] to parent\n", getpid());
	}
}
double get_result(int fd) {
        double res;
        if (sizeof(double) != read(fd, &res, sizeof(double)))
                abort_prg("read error");
        return res;
}

pid_t wait_child(int* signal_id) {
        while (0 == flag[0] && 0 == flag[1])
		;
	if (1==flag[0])	{
		*signal_id = SIGUSR1;
		flag[0] = 0;
		return child[0];
	} else {
		*signal_id = SIGUSR2;
		flag[1] = 0;
		return child[1];
	}
}

double parent_proc(int fds[2][2]) {
	int signal_id;
	pid_t cpid;
	double sum = 0.0;
	double temp;
	int id;

	int i;
	for (i = 0; i < ITER_COUNT * 2; ++i) {
		cpid = wait_child(&signal_id); //ожидание сигнала от ребенка
		id = (cpid == child[0]) ? 0 : 1;
		temp = get_result(fds[id][0]);
		sum +=temp;
		printf("[%d]\tReceived data[%.5lf]\t by signal[%d] from child[%d]\n", i, temp, signal_id, cpid);
		kill(cpid, signal_id);		//сигнал SIGUSR посылается child, который махнул флагом
		printf("Signal from parent to child[%u]\n", cpid);

	}
	
	int status;
        for(i = 0; i < 2; ++i) {
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
        if (argc < 2) {	
               	printf("You forgot to enter a value\n");
		exit(0);
	}
	sscanf(argv[1], "%lf", &x);

	double old = x;
	if(x>1) {
                x = 1/x;
                sign = -1;
		div = PI/2;
	}       
	int i;
        for(i = 0; i < 2; ++i)
               if (0 != pipe(fds[i]))
                abort_prg("can't create pipe\n");

	setup_handler();

        for(i = 0; i < 2; ++i) {
                if((child[i] = fork()) < 0)
			abort_prg("can't create child\n");
		if(child[i] == 0) {  
			child_proc(x, i, fds[i]);
                	return 0;
        	}
        }
	
        double res = parent_proc(fds);
	double ans = (div + sign*res)*DEGREES_PER_RADIAN;
        printf("\narctg(%lf) = %lf + %d*arctg(%lf))\n", old, div, sign, x);
	printf("arctg(%lf): %.7lf\n", old, ans);

        return 0;
}
           
