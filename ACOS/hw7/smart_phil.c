#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

#define SHM_NAME "/hello.shm"
#define MAX_RAND 5
#define MIN_RAND 1
#define MAX_COUNT 1024
#define REAL_MAX_VALUE 30000000
//#define usleep sleep

int cnt;

typedef struct {
	int sem[MAX_COUNT];
}stick_t;

void abort_prg(const char* err) {
	fprintf(stderr, "%s\n", err);
	exit(1);
}
void init_mas(stick_t* sticks) {
        int i;
        for(i = 0; i < cnt; ++i)
              sticks->sem[i] = 1;
}
int my_sem_wait(int* stick, sem_t* semaphore, int k){
	int iter_cnt = 0;
        assert_perror(sem_wait(semaphore));
        while(1 != *stick && iter_cnt < k){
        	iter_cnt++;
	        assert_perror(sem_post(semaphore));
                assert_perror(sem_wait(semaphore));
        }
        if(iter_cnt >= k) {
		assert_perror(sem_post(semaphore));
                return 0;
        }
	*stick = 0;
        assert_perror(sem_post(semaphore));
	return 1;

}
void my_sem_post(int* stick, sem_t* semaphore){
	assert_perror(sem_wait(semaphore));
	*stick = 1;
	assert_perror(sem_post(semaphore));
}
void child_proc(int id, int fd, sem_t* semaphore){
	stick_t* sticks = mmap(NULL, sizeof(stick_t), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        assert(MAP_FAILED != sticks);

	srand(time(NULL)^getpid()<<16);

	int wait_time;
	while(1){
		// thinkng
                wait_time = MIN_RAND + rand() % MAX_RAND;
                printf("Philosopher [%d] State: [Thinking for %d sec]\n", id, wait_time);
                usleep(wait_time);

                // hungry
                printf("Philosopher [%d] State: [HUNGRY]\n", id);

		my_sem_wait(&sticks->sem[id], semaphore, REAL_MAX_VALUE);
		printf("Philosofer [%d] take on left hand the [%d] stick\n", id, id);
		usleep(1);
		if(0 == my_sem_wait(&sticks->sem[(id+1)%cnt], semaphore, REAL_MAX_VALUE)){
			printf("roll back\n");
			my_sem_post(&sticks->sem[id], semaphore);
			continue;
		}
               	printf("Philosofer [%d] take on right hand the [%d] stick\n", id, (id + 1) % cnt);

		// eating
                wait_time = MIN_RAND + rand() % MAX_RAND;
                printf("Philosopher [%d] State: [Eating for %d sec]\n", id, wait_time);
                usleep(wait_time);

                // ending
		my_sem_post(&sticks->sem[id], semaphore);
		usleep(1);
		printf("Philosofer [%d] put the [%d] stick\n", id, id);
		my_sem_post(&sticks->sem[(id+1)%cnt], semaphore);
		printf("Philosofer [%d] put the [%d] stick\n", id, (id + 1) % cnt);
	}
	munmap(sticks, sizeof(stick_t));
}

int main(int argc, char* argv[]) {
	if(argc != 2 || 1 != sscanf(argv[1], "%d", &cnt))
		abort_prg("Bad scanf");
	if(cnt > MAX_COUNT)
		printf("number of philosophers must be less than %d", MAX_COUNT);
	
        sem_t semaphore;
        assert_perror(sem_init(&semaphore, 1, 1));

	int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
        assert(fd >= 0);
        assert_perror(ftruncate(fd, sizeof(stick_t)));

        stick_t* sticks = mmap(NULL, sizeof(stick_t), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        assert(MAP_FAILED != sticks);
        init_mas(sticks);
        munmap(sticks, sizeof(stick_t));	
	
	pid_t* childs = (pid_t*)malloc(sizeof(pid_t)*cnt);
	assert(childs);
	int i;
	for(i = 0; i < cnt; ++i) {
		if(0 > (childs[i] = fork()))
			abort_prg("Error in fork");
		if(0 == childs[i]) {
			child_proc(i, fd, &semaphore);
			return 0;
		}
	}
	
	int status;
        for(i = 0; i < cnt; ++i) {
                if(childs[i] != waitpid(childs[i], &status, 0))
                        abort_prg("Waitpid error");
                if(0 != status)
                        fprintf(stderr, "Child has some errors: %d\n", status);
        }

        free(childs);
	close(fd);
        shm_unlink(SHM_NAME);

	return 0;
}
