#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <pthread.h>

#define NUM_OF_READING_THREADS 10

pthread_mutex_t mutex;
int count = 0;

void* writer(void* args) {
	(void)args;
	for(int i = 0; i < 10; ++i) {
		pthread_mutex_lock(&mutex);
		++count;
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	pthread_exit(0);
}

void* reader(void* args) {
	(void)args;
	for(int i = 0; i < 10; ++i) {
		pthread_mutex_lock(&mutex);
		printf("counter: %d tid: %ld\n", count, pthread_self());
		pthread_mutex_unlock(&mutex);
		sleep(2); 
	}
	pthread_exit(0);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
	pthread_t w_tid;
	pthread_t r_tid[NUM_OF_READING_THREADS];
	pthread_mutex_init(&mutex, NULL);

	pthread_create(&w_tid, NULL, writer, NULL);
	for(int i = 0; i < NUM_OF_READING_THREADS; ++i) {
		pthread_create(&r_tid[i], NULL, reader, NULL);
	}

	pthread_join(w_tid, NULL);
	for(int i = 0; i < NUM_OF_READING_THREADS; ++i) {
		pthread_join(r_tid[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	return 0;
}
