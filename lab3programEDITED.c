

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

/*
*Halston Raddatz
*Bryana Craig
*CIS 452
*Greg Wolffe
*Lab 3 - Exploring interprocess communication in C. Asynchronous IPC demonstrating the use of signals for
*        simple communication. Using signal handlers, child processes, rand(), and wait(). 
*/

    //variable containing process ID
    pid_t pid;
void quitHandler (int sigNum) {

    printf (" received. That's it, I'm shutting you down...\n");

    // The kill() system call can be used to send any signal to any process
    //group or process. Upon successful completion, 0 shall be returned. Otherwise,
    // -1 shall be returned and errno set to indicate the error.
    if (kill(pid, SIGKILL) < 0) {
        //if the fork happened improperly, the PID will be negative 1 therefore it will be less than 0 and the kill
        //command will fail returning a -1.
        perror("Could not kill child.\n");
        exit(1);
    }

    exit(0);
}


void signalHandler1 (int sigNum) {
    printf ("received a SIGUSR1 signal\n");
}

void signalHandler2 (int sigNum) {
    printf ("received a SIGUSR2 signal\n");
}

int main() {

    void quitHandler (int);
    void signalHandler1 (int);
    void signalHandler2 (int);


//The C library function void signal()  sets a function to handle signal
// i.e. a signal handler with signal number sig.
//This function returns the previous value of the signal handler, or SIG_ERR on error.
    signal(SIGUSR1, signalHandler1);
    signal(SIGUSR2, signalHandler2);
    

    //forking process and setting variable pid to return value of fork(). -1 fork failed, 0 if child, pid if parents
    pid = fork();

//pid is 0 if process is the child
    if (pid == 0) {


        while (1) {

            int randTime = rand() % 5 + 1;

            //sleep() makes the calling thread sleep until parameter time has
            //elapsed or a signal arrives which is not ignored.
            sleep(randTime);

            int random = rand() % 2 + 1;

            //getppid() returns the process ID of the parent of the calling process.
            pid_t ppid = getppid();

            if (random == 1){

                // The kill() system call can be used to send any signal to any process
                //group or process.
                //Upon successful completion, 0 shall be returned. Otherwise,
                // -1 shall be returned and errno set to indicate the error.
                //sends signal for sigUSR1 to parent unless error
                if (kill(ppid, SIGUSR1) < 0) {
                    perror("killing SIGUSR1 failed\n");
                    exit(1);
                }
            }

            if (random == 2){
                //sends signal for sigUSR2 to parent unless error
                if (kill(ppid, SIGUSR2) < 0) {
                    perror("killing SIGUSR2 failed\n");
                    exit(1);
                }
            }
        }
    }

    else {

        printf("spawned child PID# %d\n", pid);

        //receives interrupt and sends the interrupt signal to handler
        signal (SIGINT, quitHandler);

        while (1) {
            printf("waiting...       ");

            fflush(stdout);

            //The pause() library function causes the invoking process(or thread)
            //to sleep until a signal is received that either terminates it or
            // causes it to call a signal-catching function.
            pause();
        }
    }
    return 0;
}
