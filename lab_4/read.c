#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sem.h>

#define FILE "read.c"

int shmem_id, semid;
struct sembuf sem_lock = {0,-1,0}, sem_open = {0,1,0};


int main() {
   
    char *addr;
    
    key_t IPCkey = ftok(FILE, 1);
    key_t semkey = ftok("/tmp", 1);
    
    shmem_id = shmget(IPCkey, 32, 0666);
    if (shmem_id == -1) {
	perror("shmem id errno:%d\n");
	exit(0);
    }

    semid = semget(semkey, 1, 0666);
    if (semid == -1) {
	perror("Semaphore open error");
	exit(0);
    }


    if ((addr = shmat(shmem_id, NULL, 0)) == (char *)-1) {
	perror("SharedMemory link error\n");
        exit(0);
    }

    while(1) {   
        time_t timer = time(0);
	semop(semid, &sem_open, 1);
	printf("\ntime_Reader = %spid_Reader = %d\n", ctime(&timer), getpid());
	printf("\n%s", addr);
	semop(semid, &sem_lock,1);

        sleep(1);
    }
    return 0;
}
