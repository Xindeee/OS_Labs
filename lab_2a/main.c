#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int fd[2]; 

    if (pipe(fd) == -1) {
        perror("Failed pipe open");
        exit(0);
    }
    char str[100];
    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("Fork failed");
            exit(0); 
        case 0:
            // child 
	    close(fd[1]); 
            sleep(5);


            if((read(fd[0], str, 100)) == -1) {
		    perror("Failed to read pipe");
		    exit(0);
	    }
            close(fd[0]);
            time_t timer = time(0);
           
            printf(" Child time:   %s%s \n", ctime(&timer), str);
            exit(0);
        default:
            //parent 
	    close(fd[0]);

            time_t p_timer = time(0);
	    sleep(5);
	    sprintf(str, "%s %s %s %d", " Parent time: ", ctime(&p_timer), "Parent pid = ", getpid());
            write(fd[1], str, 100);
            close(fd[1]);

            if (waitpid(pid, 0, 0) == -1) {
                perror("waitpid");
                exit(0);
            }
    }
    return 0;
}
