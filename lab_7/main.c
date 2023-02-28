#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <pthread.h>

#define NUM_OF_READING_THREADS 10

pthread_rwlock_t rwlock;
int count = 0;

void* writer(void* args) {
	(void)args;
	for(int i = 0; i < 10; ++i) {
		pthread_rwlock_wrlock(&rwlock);
		++count;
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
	pthread_exit(0);
}

void* reader(void* args) {
	(void)args;
	for(int i = 0; i < 10; ++i) {
		pthread_rwlock_rdlock(&rwlock);
		printf("counter: %d tid: %ld\n", count, pthread_self());
		pthread_rwlock_unlock(&rwlock);
		sleep(1); 
	}
	pthread_exit(0);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	pthread_t w_tid;
	pthread_t r_tid[NUM_OF_READING_THREADS];
	pthread_rwlock_init(&rwlock, NULL);

	pthread_create(&w_tid, NULL, writer, NULL);
	for(int i = 0; i < NUM_OF_READING_THREADS; ++i) {
		pthread_create(&r_tid[i], NULL, reader, NULL);
	}

	pthread_join(w_tid, NULL);
	for(int i = 0; i < NUM_OF_READING_THREADS; ++i) {
		pthread_join(r_tid[i], NULL);
	}

	pthread_rwlock_destroy(&rwlock);
	return 0;
}
