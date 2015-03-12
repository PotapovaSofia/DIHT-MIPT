#include <stdlib.h>
#include <stdio.h>
#include "threadpool.h"
#define THREAD_COUNT 5


void abort_prg(const char* msg) {
	fprintf(stderr, "error: %s\n", msg);
	exit(1);	
}

void task(void* data) {
	printf("[%d]Thread working: %d\n", (int)data, (int)pthread_self());
}

int main(int argc, char* argv[]) {
	thread_pool tp;
	if (0 != tp_init(&tp, THREAD_COUNT))
		abort_prg("can't create pool");

	int i;
	for (i = 0; i < 10; ++i) {
		if (0 != tp_add_task(&tp, task, (void*)i))
			abort_prg("can't add task");
	}
	
	if (0 != tp_join(&tp))
		abort_prg("can't join all tasks");

	if (0 != tp_destroy(&tp))
		fprintf(stderr, "can't destroy threadpool\n");

	return 0;
}
