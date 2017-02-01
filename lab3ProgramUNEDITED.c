

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void sigHandlerQuit (int);
void sigHandlerUSR1 (int);
void sigHandlerUSR2 (int);

pid_t pid; 


int main() {

	signal(SIGUSR1, sigHandlerUSR1);
	signal(SIGUSR2, sigHandlerUSR2);

	if ((pid = fork()) < 0) { 
		perror ("fork failed"); 
		exit(1); 
	}
	
	else if (pid == 0) { 

		while (1) {
			
			int randTime = rand() % 5 + 1;
	
			sleep(randTime);

			int randUSR = rand() % 2 + 1;
			
			pid_t ppid = getppid();

			if (randUSR == 1){
				if (kill(ppid, SIGUSR1) < 0) {
					perror("kill SIGUSR1 failed\n");
					exit(1);
				}
			}
			
			if (randUSR == 2){
				if (kill(ppid, SIGUSR2) < 0) {
					perror("kill SIGUSR2 failed\n");
					exit(1);
				}
			}
		}
	}

	else {

		printf("spawned child PID# %d\n", pid);
    
		signal (SIGINT, sigHandlerQuit);
    
		while (1) {
			printf("waiting...       ");

			fflush(stdout);
			
			pause();
		}
	}
	return 0;
}

void sigHandlerQuit (int sigNum) {
    
	printf (" received. That's it, I'm shutting you down...\n");
    
	if (kill(pid, SIGKILL) < 0) {
		perror("Could not kill child.\n");
		exit(1);
	}

	exit(0);
}

void sigHandlerUSR1 (int sigNum) {
    printf ("received a SIGUSR1 signal\n");
}

void sigHandlerUSR2 (int sigNum) {
    printf ("received a SIGUSR2 signal\n");
}
