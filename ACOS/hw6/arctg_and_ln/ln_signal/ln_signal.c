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
#define PI 3.1415926
#define ENUMBER 2.7182818284

pid_t child[2];
volatile sig_atomic_t flag[2] = {0,0};

typedef struct {
        double r;
        double i;
}cmpl;


void abort_prg(const char* error) {
        perror(error);
        exit(1);
}

cmpl cmpl_plus(cmpl a, cmpl b)  {
        cmpl res;
        res.r = a.r + b.r;
        res.i = a.i + b.i;
        return res;
}

cmpl cmpl_mult(cmpl a, cmpl b)  {
        cmpl res;
        res.r = a.r * b.r - a.i * b.i;
        res.i = a.r * b.i + a.i * b.r;
        return res;
}

cmpl cmpl_div(cmpl a, cmpl b)   {
        cmpl res;
        res.r = (a.r * b.r + a.i * b.i) / (b.r * b.r  + b.i * b.i);
        res.i = (a.i * b.r - a.r * b.i) / (b.r * b.r + b.i * b.i);
        return res;
}

cmpl cmpl_deg(cmpl a, int n)    {
        cmpl res;
        res.r = 1;
        res.i = 0;
        int j;
        for(j = 0; j < n; ++j)
                res = cmpl_mult(a, res);
        return res;
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
	} else 	{
		while(0 == flag[1])
			;
		flag[1] = 0;
	}
}

void child_proc(cmpl x, int id, int fds[2]) {
        close(fds[0]);
	int signal_id = (0 == id) ? SIGUSR1 : SIGUSR2;
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
		if (sizeof(cmpl) != write(fds[1], &temp, sizeof(cmpl)))
        	        abort_prg("write error");
	        kill(getppid(), signal_id); //сигнал SIGUSR посылается родительскому процессу
        	printf("Signal fom chiild[%u] to parent\n", getpid());
		wait_parent(id);
		t.r = cmpl_mult(t, p).r;
                t.i = cmpl_mult(t, p).i;
        }

/*	int i;
        for (i = 1; i < ITER_COUNT; ++i) {
		wait_parent(id);	//ожидание отклика от родительского процесса 
		if(0 == id)
			t*=x*x*(2*i-1)/(2*i+1);
		else
			t*=x*x*2*i/(2*i+2);
		
        	if (sizeof(double) != write(fds[1], &t, sizeof(double)))
                	abort_prg("write error");
		kill(getppid(), signal_id); //сигнал SIGUSR посылается родительскому процессу
		printf("Signal fom chiild[%u] to parent\n", getpid());
	}
*/
}
cmpl get_result(int fd) {
        cmpl res;
        if (sizeof(cmpl) != read(fd, &res, sizeof(cmpl)))
                abort_prg("read error");
        return res;
}

pid_t wait_child(int* signal_id) {
        while (0 == flag[0] && 0 == flag[1])
		;
	if (1==flag[0]) {
		*signal_id = SIGUSR1;
		flag[0] = 0;
		return child[0];
	} else	{
		*signal_id = SIGUSR2;
		flag[1] = 0;
		return child[1];
	}
}

cmpl parent_proc(int fds[2][2]) {
	int signal_id;
	pid_t cpid;
	int id;

	cmpl sum;
        sum.r = 0;
        sum.i = 0;
        cmpl temp, t;
        int i;
        for (i = 0; i < ITER_COUNT / 2; ++i)    {
		cpid = wait_child(&signal_id); //ожидание сигнала от ребенка
                id = (cpid == child[0]) ? 0 : 1;
                temp = get_result(fds[id][0]);
                sum.r = cmpl_plus(sum, temp).r;
                sum.i = cmpl_plus(sum, temp).i;
		printf("[%d]\tReceived data[%.5lf + i * %lf]\t by signal[%d] from child_pid[%d]\n", i, temp.r, temp.i, signal_id, cpid);
                kill(cpid, signal_id); //сигнал SIGUSR посылается child, который махнул флагом
                printf("Signal from parent to child[%u]\n", cpid);
          
        }
/*
	int i;
	for (i = 0; i < ITER_COUNT * 2; ++i) {
		cpid = wait_child(&signal_id); //ожидание сигнала от ребенка
		id = (cpid == child[0]) ? 0 : 1;
		temp = get_result(fds[id][0]);
		sum +=temp;
		printf("[%d]\tReceived data[%.5lf]\t by signal[%d] from child_pid[%d]\n", i, temp, signal_id, cpid);
		kill(cpid, signal_id);		//сигнал SIGUSR посылается child, который махнул флагом
		printf("Signal from parent to child[%u]\n", cpid);
	}
*/	
	int status;
        for(i = 0; i < 2; ++i) {
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
  	double x= 0.0;
	double y= 0.0;	

	int fds[2][2];
        if (argc < 2) {	
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
        if(x < 0)       {
                x = x - 2*x;
                flag = 1;
        }

	//ln(x*e^(-k)) = ln(x) + ln(e^(-k));
        int k = 0, fl = 0;
        if((sqrt((x - 1)*(x - 1) + y*y)) > 1)   {
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


	int i;
        for(i = 0; i < 2; ++i)
               if (0 != pipe(fds[i]))
                abort_prg("can't create pipe\n");

	setup_handler();

        for(i = 0; i < 2; ++i) {
                if((child[i] = fork()) < 0)
			abort_prg("can't create child\n");
		if(child[i] == 0) {  
			child_proc(c, i, fds[i]);
                	return 0;
        	}
        }
	
        cmpl res = parent_proc(fds);

        if(fl)   res.r += abs(k);
        if(flag) res.i = PI - res.i;

        printf("Result:\t%lf + i * %lf\n", res.r, res.i);

        return 0;
}
           
