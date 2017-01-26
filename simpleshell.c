/*
*Halston Raddatz
*Bryana Craig
*CIS 452
*Greg Wolffe
*Lab 2 - simple shell program
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define Buffer 8400
#define Args 256


int userInput(char input[], int inputLength, char* args[]);
void prepareArgs(char input[], char* args[]);
void command(char* args[]);

//preparing the arguments
void prepareArgs(char input[], char* args[]) {

    char* tok;
    int i = 0;

    //using space as a delimiter
    tok = strtok(input, " ");

    while (tok != NULL) {
        char* arg = malloc(sizeof(*arg) * (strlen(tok) + 1));
        strcpy(arg, tok);
        args[i] = arg;
        tok = strtok(NULL, " ");

        i++;
    }
    args[i] = NULL;
}

//receiving the users input
int userInput (char input[], int inputLength, char* args[]) {

    //checking if input is blank/null
    if (fgets(input, inputLength, stdin) != NULL) {

        //newline check
        input[strcspn(input, "\n")] = 0;

        //checking if user wants to quit
        if (strcmp(input, "quit") == 0)
            exit(0);

        //if all checks pass, sends the input to the
        //prepareArgs function and returns 1
        prepareArgs(input, args);

        return 1;
    }
    return 0;
}

void command (char* args[]) {

    int status = 0;

    pid_t pid;
    pid = fork();

    //fork should return value above 0
    //if it doesnt, something went wrong and should exit
    if (pid < 0) {

        exit(1);

    } else if (pid) {

        // struct for resource usage of programs process
        struct rusage res;

        //parent waits for child to complete
        wait3(&status, 0, &res);

        struct timeval uTime = res.ru_utime;
        time_t uTimeSec = uTime.tv_sec;
        long int uTimeMicroSec = uTime.tv_usec;
        printf("\nCPU time used: %ld seconds %ld microseconds\n", uTimeSec, uTimeMicroSec);

        long int ics = res.ru_nivcsw;
        printf("Involuntary Context Switches: %ld\n", ics);

    } else {

        if (execvp(args[0], args) < 0) {
            exit(0);
        }
        exit(status);
    }
}
int main() {

    char input [Buffer];
    char* args[Args];

    while(1) {
        printf("SimpleShell: ");

        //userInput returns 0 if the user entered nothing
        //returns 1 otherwise
        if (!userInput(input, Buffer, args)) {
            printf("Invalid command\n");
        }
        command(args);
    }
    return 0;
}
