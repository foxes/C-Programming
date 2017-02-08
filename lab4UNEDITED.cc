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
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <signal.h>

#define RUNNING 1

using namespace std;

// Prototypes
void *find_file(string arg);
void sig_handler(int sig_num);

int serviced;
bool accepting_input;

int main(int argc, char* argv[]) {
    serviced = 0;
    accepting_input = true; 
    signal(SIGINT, sig_handler);
    while (RUNNING) {
        string input;
        if (accepting_input) {
            cout << "Enter a filename: " << flush;
            cin >> input;
            thread file(find_file, input); 
            // Thread can run independently of dispatcher
            // and free memory after finishing
            file.detach(); 
        }
    }
    return 0;
}

void sig_handler(int sig_num) {
    cout << " exit received. Printing stats." << endl;
    accepting_input = false;
    cout << "We handled " << serviced << " inputs." << endl;
    exit(0);
}

void *find_file(string arg) {
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
    // We handled another file. 
    serviced++;
    return NULL;
}
