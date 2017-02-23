/* 
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * Lab 6
 * Protecting the critical sections to prevent memory access conflicts from causing inconsistencies in the output.
 * Using semphaores to synchronize two processes & performing cleanup operations after.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SIZE 16

int semId = -1;

//function prototype for function that will do cleanup, "called" by sighandler.
void quit();
//function that will "cleanup" so we dont have any stray
void quit()
{
    if(semId != -1)
    {
        //semctl(int semid, int semnum, int cmd,...)
	//semtcl performs the operation specifid by cmd on the semaphore set 
	//identified by semid or the semnum-th semphaore of that set (the semaphores in a set are #'d starting at 0)
	//IPC_RMID marks the segment to be destroyed, so this function 
	/*effectively this line "cleans up" the semaphore to prepare for program exit later in the function.*/
        semctl(semId, 0, IPC_RMID);
    }

    exit(0);
}

//main function of program, takes arguements 
int main (int argc, char *argv[])
{
   int status;
   long int i, loop, temp, *shmPtr;
   int shmId;
   pid_t pid;

   //"installing" the sighandler and linking it to the quit() function
   signal(SIGINT, quit);


   loop = atoi(argv[1]);

   //segmet(key_t key, int nsems, int semflg)
   //semget system call returns the semaphore set identifier associated with the arguement *key*
   //a new set of nsems semphaores is created if *key* has the value IPC_PRIVATE or if no exisitng
   //nsems = # of arguements in the sempahore array
   //semaphore set is associated with *key* and IPC_CREAT is specified in semflg
   //if semflg specifies both IPC_CREAT and IPC_EXCL and a semphaore set already exists for key, the semget
   //fails with errno set to EEXIST.
   /* Here we are getting the semaphore that will be used to synchronize our processes */
   /* this line was given to us in the lab description except for addition of more semflgs) */
   semId = semget(IPC_PRIVATE, 1, 00600 | IPC_CREAT | IPC_EXCL);

   //semctl(int semid, int semnum, int cmd,...)
   //semtcl performs the operation specifid by cmd on the semaphore set 
   //identified by semid or the semnum-th semphaore of that set (the semaphores in a set are #'d starting at 0)
   /*IPC_SET writes values of members of the semid to the kernel associated with the sempaphore */
   /*Here we are initializing our semaphore. */
   /* initializing the semaphore set references by the previously obtained semID handle */
   semctl(semId, 0, SETVAL, IPC_SET);


 
   //creating sembuf wait and signal structs so we can use semop() later
   struct sembuf waitbuf;
   waitbuf.sem_num = 0;
   waitbuf.sem_op = -1;  //-1 for locking our semaphore
   waitbuf.sem_flg = 0;  

   struct sembuf signalbuf;
   signalbuf.sem_num = 0;
   signalbuf.sem_op = 1;    //1 for releasing our semaphore
   signalbuf.sem_flg = 0;  

 
    //shmget(): this function creates a shared memory segment.  It initializes a kernel data structure 
    //to manage the resource and returns a resource ID to the user.  
    //This ID, or handle, is used by any process wishing to access the shared segment.
    //shmget() parameters: int shmget(key_t key, size_t size, int shmflg);
    //IPC_CREAT = Create the segment if it doesn't already exist in the kernel.
    //S_IRUSR = Read permission bit for the owner of the file.
    //S_IWUSR = Write permission bit for the owner of the file
    //On success, a valid shared memory identifier is returned.  On error,-1 is returned,
    // Catches errors in creation and checking of oldMessage and write permissions
   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
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
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   shmPtr[0] = 0;
   shmPtr[1] = 1;

   if (!(pid = fork())) {
      for (i=0; i<loop; i++) {


         //int semop(int semid, struct sembuf *sops, size_t nsops)
	 //semop performs operations on selected semaphores in the set indicated by semid. 
         //sops is a pointer to a user-defined array of semaphore operation structures
	 //nsops is the number of such structures in the array
	 /* we're using the &waitbuf struct to lock on to our semaphore */
         semop (semId, &waitbuf, 1);

         // swap the contents of shmPtr[0] and shmPtr[1]
         temp = shmPtr[0];
         shmPtr[0] = shmPtr[1];
         shmPtr[1] = temp;

         /* we're using the &signalbuf struct to release our semaphore */
         semop (semId, &signalbuf, 1);
    }

      exit(0);
   }
   else
      for (i=0; i<loop; i++) {

         /* we're using the &waitbuf struct to lock on to our semaphore */
         semop (semId, &waitbuf, 1);

         // swap the contents of shmPtr[1] and shmPtr[0]
         temp = shmPtr[1];
         shmPtr[1] = shmPtr[0];
         shmPtr[0] = temp;

         /* we're using the &signalbuf struct to release our semaphore */
         semop (semId, &signalbuf, 1);
      }

   //The wait() system call suspends execution of the current process until one of its children terminates. 
   //The call wait(&status) is equivalent to: waitpid(-1, &status, 0);
   wait (&status);

   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   //calls our nice clean exit function
   quit();
}
