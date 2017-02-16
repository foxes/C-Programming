/* 
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * Lab 5
 * Implementing a writer program that creates a shared memory segment, repeatedly
 * accepting an arbitrary user input string from the terminal and writing it into shared memory.
 * attaches to a Reader program; which will repeatedly read each new string from the shared memory
 * and display the string to the screen.
 */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


using namespace std;

struct sharedMemoryData {
    int oldMessage;
    bool newMessage;
    char user_input[2048];
};

int main (int argc, char* argv[]) {

    
    int shmId;

    struct sharedMemoryData* sharedMemoryPointer;
    string input;

    //ftok: convert a pathname and a project identifier to a System V IPC key
    //ftok paramters: key_t ftok(const char *pathname, int proj_id);
    key_t file_to_key = ftok("../", 'K');

    //shmget(): this function creates a shared memory segment.  It initializes a kernel data structure 
    //to manage the resource and returns a resource ID to the user.  
    //This ID, or handle, is used by any process wishing to access the shared segment.
    //shmget() parameters: int shmget(key_t key, size_t size, int shmflg);
    //IPC_CREAT = Create the segment if it doesn't already exist in the kernel.
    //S_IRUSR = Read permission bit for the owner of the file.
    //S_IWUSR = Write permission bit for the owner of the file
    //On success, a valid shared memory identifier is returned.  On error,-1 is returned,
    // Catches errors in creation and checking of oldMessage and write permissions
    if ((shmId = shmget (file_to_key, sizeof (sharedMemoryData), IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("ERROR creating shared memory segment\n");
        exit (1);
    }

    //attach shared memory segment to data space
    //shmat() attaches the System V shared memory segment identified by
    //shmid to the address space of the calling process.
    //shmat paramters: shmat(int shmid, const void *shmaddr, int shmflg)
    //shmid is a unique positive integer created by a shmget() system call and associated with a segment of shared memory
    //shmaddr Points to the desired address of the shared memory segment.
    //shmflg Specifies a set of flags that indicate the specific shared memory conditions and options to implement.
    //Catches errors in the attachment of the shared memory segment
    if ( (sharedMemoryPointer = (sharedMemoryData*) shmat(shmId, 0, 0)) == (void *) -1) {
        perror ("error attaching memory segment\n");
        exit (1);
    }
    
    //keeps accepting the input until user enters "quit"
    while(input.compare("quit") != 0) {
        cin >> input;
        strcpy(sharedMemoryPointer->user_input, input.c_str());
        //setting boolean newMessage so reader can determine whether it has already printed message to terminal
        sharedMemoryPointer->oldMessage = 0;
        sharedMemoryPointer->newMessage = true;
    }

    //detach shared memory segment
    //shmdt() detaches the shared memory segment located at the address
    //specified by shmaddr from the address space of the calling process
    // Catches errors while detaching shared memory segment found at sharedMemoryPointer
    //On success shmdt() returns 0; on error -1 is returned,
    if (shmdt (sharedMemoryPointer) < 0) {
        perror ("error detaching memory\n");
        exit (1);
    }

    //shmctl parameters: int shmctl(int shmid, int cmd, struct shmid_ds *buf)
    //shmctl() performs the control operation specified by cmd on the
    //System V shared memory segment whose identifier is given in shmid.
    //The buf argument is a pointer to a shmid_ds structure.
    //cmd = IPC_RMID tells the function to mark the segment to be DESTROYED
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("error destroying memory segment");
        exit (1);
    }

    return 0;
}
