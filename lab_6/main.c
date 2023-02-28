#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define THREAD_COUNT 10

pthread_rwlock_t lock;
int count = 0;

void programExit(int sig) {
	printf("\n");
	exit(0);
}

void *readThread(void *num) {
	while (1) {
    		pthread_rwlock_rdlock(&lock);
	        printf("Thread #%d | TID = %lx | Count = %d\n",
		       	*(int*)num, (long)pthread_self(), count);
    	
		pthread_rwlock_unlock(&lock);
		sleep(1);
  	}
}

void *writeThread(void *arg) {
	while (1) {
		sleep(rand() % 10);
		pthread_rwlock_wrlock(&lock);
	   	count++;
		printf("Count = %d\n", count);
	 	sleep(rand() % 10);
	   	pthread_rwlock_unlock(&lock);
  	}
}

int main() {
	signal(SIGINT, programExit);
	pthread_t threads[THREAD_COUNT + 1];
	int threadArr[THREAD_COUNT];

	pthread_rwlock_init(&lock, NULL);

	for (int i = 0; i < THREAD_COUNT; i++) {
        	threadArr[i] = i;
	    	pthread_create(&threads[i], NULL, readThread, &threadArr[i]);
	}

	pthread_create(&threads[THREAD_COUNT], NULL, writeThread, NULL);

	for (int i = 0; i < THREAD_COUNT + 1; i++) 	
	    	pthread_join(threads[i], NULL);

	pthread_rwlock_destroy(&lock);
	printf("end\n");
	return 0;
}
