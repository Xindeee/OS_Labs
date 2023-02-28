#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    unlink("fifo");

    if (mkfifo("fifo", 0666) == -1) {
        perror("FIFO create failure");
        exit(0);
    }

    pid_t pid = fork();
    char str[100];
    switch (pid) {
        case -1:
            perror("Fork failure");
            exit(0);
        case 0:
            // Child
            ;
	    int fd_fifo = open("fifo", O_RDONLY);
            if (fd_fifo == -1) {
                perror("FIFO open failure\n");
                exit(0);
            }

           
	    if ((read(fd_fifo, str, 100)) == -1) {
		perror("Failed to read FIFO");	   
		exit(0);
	    }
	    close(fd_fifo);
	    time_t timer = time(0);            
	    printf(" Child time:   %s%s \n", ctime(&timer), str);
            exit(0);
        default:
           // Parent
     	    ;
	    fd_fifo = open("fifo", O_WRONLY);
            if (fd_fifo == -1) {
                perror("FIFO open failure\n");
                exit(0);
            }
	    time_t p_timer = time(0);
	    sleep(5);
	    sprintf(str, "%s %s %s %d", " Parent time: ", ctime(&p_timer), "Parent pid = ", getpid());
	    write(fd_fifo, str, 100);
	    close(fd_fifo);

            if (waitpid(pid, 0, 0) == -1) {
                perror("waitpid");
                exit(0);
            }
    }

    return 0;
}
