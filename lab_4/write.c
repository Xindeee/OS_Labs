#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/sem.h>

#define FILE "read.c"

int shm_id, semid;
struct sembuf sem_lock = {0, -1, 0}, sem_open = {0,1,0};

void funcExit(int sig) {
    printf("\n");
    struct shmid_ds *buf = 0;
    shmctl(shm_id, IPC_RMID, buf);
    exit(0);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, funcExit);
    char * addr;
    key_t IPCkey = ftok(FILE, 1);
    key_t semkey = ftok("/tmp", 1);
    shm_id = shmget(IPCkey, 32, IPC_CREAT | 0666);
    semid = semget(semkey, 1, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Can't create shared memory\n");
        exit(0);
    }
    if (semid == -1) {
    	perror("Semaphore create error\n");
	exit(0);
    }


    

    if ((addr = shmat(shm_id, NULL, 0)) == (char *)-1) {
        perror("shmat error\n");
        exit(0);
    }

    time_t timer = time(0);
    time_t buf_timer = timer;
    int check = 1;
    if (argc > 1) {
	if (strcmp(argv[1], "-f") == 0) 
	    check = 1;
	else {
	    printf("This key is not processed\n");
	    exit(0);
	}
    } else {
    	if (strlen(addr) != 0) {
        printf("Sending process has already been started. Error. To clear memory, run programm with flag -f\n");
	check = 0;
        exit(0);
   	}
    }
    while(check > 0) {
        timer = time(0);
	if (timer != buf_timer) {
            semop(semid, &sem_open, 1);
	    buf_timer = timer;
	    sprintf(addr, "time_Writer = %spid_Writer = %d\n", ctime(&timer), getpid());
	    sleep(1);
	    semop(semid, &sem_lock, 1);
    	}
    }
    return 0;
}

