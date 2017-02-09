/*
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * Lab 4
 * Simulating the thread execution manifested by a multi-thread fileserver process.
 * User inputs a string simulating the name of a file to access, program spawns
 * a child thread and sends the user supplied filename to it. The program then
 * immediately repeats the input/spawn sequence.
 */

#include <iostream>
#include <thread>

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

//function prototypes
void sig_handler(int sig_num);
void *fileLocating(string arg);


int filesFound;
bool inputOn;

int main(int argc, char* argv[]) {

    filesFound = 0;
    inputOn = true;

    //"installing" the signal handler for ctrl+c to print statistics
    signal(SIGINT, sig_handler);

    while (1) {
        string input;
        if (inputOn) {

	    //flush makes sure the file stream is updated with the data
	    //endl puts a newline and then uses flush
	    //since we don't want an additional newline, just use flush
            cout << "Enter a filename: " << flush;
            cin >> input;

	    //spawning a new thread that calls fileLocating(input)
            thread file(fileLocating, input);

            //detatch is a member function of thread header
	    //detches the thread represented by the object from the calling
	    //function, allowing them to execute independently from eachother.
	    //both threads continue without blocking nor synchronizing in anyway.
	    //when the thread ends execution, its resources are automatically released
	    //after a call to this function, the thread becomes non-joinable
            file.detach();
        }
    }
    return 0;
}

//sig_handler, prompts via ctrl+c (when program exits)
//prints stats held by "filesFound" variable
void sig_handler(int sig_num) {

    inputOn = false;
    cout << "we found " << filesFound << " files. Exiting." << endl;
    exit(0);
}


//main function of program, seeds a random number to the 
void *fileLocating(string arg) {

    //rand() % 101 will find a random number between 0 and 100
    int probability = rand() % 101;

    //with 80% probability, sleep for 1 second.
    //This simulates the scenario that the Worker thread has found the desired file in the disk cache and serves it up quickly.
    if (probability > 20) {
        sleep(1);
        cout << endl << "Found " << arg << "in 1 second." << endl;

    }

    //With 20% probability, sleep for 7-10 seconds (randomly).
    //This simulates the scenario that the worker thread has not found the requested file in the disk cache
    //and must block while it is read in from the hard drive.
    else {
	
        sleep((rand() % 4) + 7);
        cout << endl << "Found " << arg << "in 7 seconds." << endl;
    }

    //Whether the file is found in 1 or 10 seconds, we still found another file. so increment the variable by 1.
    filesFound++;
    return NULL;
}
