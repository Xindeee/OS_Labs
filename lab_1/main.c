#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

void exitFunc() {
	printf("Exit from %4d\n", getpid());
}

void sigTermHandler(int signum) {
	printf("    %d SIGTERM caught\n", getpid());
	printf("Information about signal: %d, (%s)\n", signum, strsignal(signum));
}

void sigIntHandler(int signum) {
	printf("    %d SIGINT caught\n", getpid());
	printf("Information about signal: %d, (%s)\n", signum, strsignal(signum));
}

int main(int argc, char** argv) {
	(void)argv;
	(void)argc;

	signal(SIGINT, sigIntHandler);

	struct sigaction sigTermAct;
	sigTermAct.sa_handler = sigTermHandler;
	sigemptyset(&sigTermAct.sa_mask);
	sigTermAct.sa_flags = 0;

	sigaction(SIGTERM, &sigTermAct, NULL);

	atexit(exitFunc);

	pid_t res = fork();
	switch(res) {
		case -1:
			fprintf(stderr, "Fork error: %d", errno);
			exit(-1);
		case 0:
			printf("[CHILD] PID: %4d PPID: %4d %d\n", getpid(), getppid(), errno);
			sleep(20);
			exit(1);
		default: {
			printf("[PARENT] PID: %4d PPID: %4d %d\n", getpid(), getppid(), errno);
			sleep(20);
			int res = 0;
			int w_res = wait(&res);
			if (WIFSIGNALED(res) != 0) {
				printf("[PARENT] Wait return: %d Signal: %d\n", w_res, WTERMSIG(res));
			}
			exit(WEXITSTATUS(res));
		}
	}

	return 0;
}