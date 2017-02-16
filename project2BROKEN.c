
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>

/* 
 * Halston Raddatz
 * CIS 452
 * Greg Wolffe
 * Project 2
 * Streamed Vector Processing
 * The intende usage of this project is to write a C program that perform subtraction 
 * on two streams of binary numbers. 
 * 
 */


int initial_pause = 1;

void pause_handler(int sigNum);
void exit_handler(int sigNum);

void complement_processor(char *number, int binary_len);
void increment_processor(char *number, int binary_len);
void add_processor(char *number_one, char *number_two, char *total, int binary_len);

int main(int argc, char **argv) {

   signal (SIGINT, pause_handler);

   // Verify command line parameters
   if (argc != 4) {
      fprintf(stdout, "Check your arguements: fileA, fileB, #-length \n");
      exit(1);
   }


   /**
    * pid_t var to store output of fork
    */
   pid_t pid;

    //storing arguements from user
    char *fileA = argv[1];
    char *fileB = argv[2];
    //atoi() function converts a string into an integer & returns that integer
    int binary_length = atoi(argv[3]);

    fprintf(stdout, "INPUT A: %s\n", fileA);
    fprintf(stdout, "INPUT B: %s\n", fileB);
    fprintf(stderr, "#-length: %d\n", binary_length);

   /**
    * Store status of wait()
    */
   int status;

   //setting up pipes
   //The arrays pipe_one & pipe_two is used to return two file descriptors referring to the ends of the pipe. 
   //pipe_one[0] & pipe_two[0] refer to the read end of their pipes, respectively. Pipe_one[1] & pipe_two[1] 
   //refers to the write end of the pipe. 
   //Data written to the write end of the pipe is buffered by the kernel until it is read from the read end of the pipe.
   int pipe_one[2];
   int pipe_two[2];
   pipe(pipe_one);
   pipe(pipe_two);

      
   int i;
   for (i = 0; i < 2; i++) {

     //System call fork() is used to create processes. It takes no arguments and returns a process ID. 
     //The purpose of fork() is to create a new process, which becomes the child process of the caller. 
     //After a new child process is created, both processes will execute the next instruction following the fork() system call.
     pid = fork();

     //fork() returns a zero to the newly created child process.
     //if new child, do this
     if (pid == 0) {
         printf("spawned child PID# %d\n", getpid());
         printf("interator = %d\n", i);


	//if first run of loop, we need to create the increment
         if (i == 0) {

            // Spawn Increment Process

            char pipe_buffer[binary_length + 1];

	    //closing the write-end of pipe_one
            //closing the read-end of pipe_two
            close(pipe_one[1]);
            close(pipe_two[0]);

            while(1) {
	
	       //int  read(  int  handle,  void  *buffer,  int  nbyte );
	       //The read() function attempts to read nbytes from the file associated with handle, 
	       //and places the characters read into buffer.
	       //If no process has the pipe open for writing, read() shall return 0 to indicate end-of-file.
               if (read(pipe_one[0], pipe_buffer, sizeof(pipe_buffer) + 1) == 0) {
                 break;
               }

               fprintf(stdout, "Incrementer: Read binary string from PIPE: %s    ", pipe_buffer);

               increment_processor(pipe_buffer, binary_length);

               fprintf(stdout, "Incrementer: Wrote binary string to PIPE: %s\n", pipe_buffer);

	       //int  write(  int  handle,  void  *buffer,  int  nbyte  )
	       //The write() function attempts to write nbytes from buffer to the file associated with handle.
               //The function returns the number of bytes written to the file. A return value of -1 indicates an error
               write(pipe_two[1], pipe_buffer, sizeof(pipe_buffer) + 1);
            }

            close(pipe_one[0]);
            close(pipe_two[1]);


         } else {

            // Spawn Adder Process
            FILE *input_file, *output_file;

            char pipe_buffer[binary_length + 1];
            char input_file_buffer[binary_length + 1];

            //closing the write-end of pipe_one
	    //closing the read-end of pipe_one
	    //closing the write-end of pipe_two
            close(pipe_one[1]);
            close(pipe_one[0]);
            close(pipe_two[1]);

            input_file = fopen(fileA, "r");
            output_file = fopen("output.dat", "w");

            while(1) {

               read(pipe_two[0], pipe_buffer, sizeof(pipe_buffer) + 1);

               if (fgets(input_file_buffer, binary_length + 3, input_file) == NULL) {
                  break;
               }

               // clean input of newlines and CR
               input_file_buffer[strcspn(input_file_buffer, "\r\n")] = 0;

               fprintf(stdout, "Adder: Read binary string from Pipe: %s    ", pipe_buffer);
               fprintf(stdout, "Adder: Read binary string from FileA: %s   ", input_file_buffer);

               char total[binary_length + 1];

               add_processor(pipe_buffer, input_file_buffer, total, binary_length);

               fprintf(stdout, "Adder: Write binary string to Output: %s\n", pipe_buffer);
               fprintf(output_file, "%s\n", pipe_buffer);
            }

            close(pipe_two[0]);
         }

         exit(0);
      }
   }

   // Parent Process - Complementer

   FILE *input_file;

   char file_buffer[binary_length + 1];

   //closing the read-end of pipe_one
   //closing the read-end of pipe_two
   //closing the write-end of pipe_two
   close(pipe_one[0]);
   close(pipe_two[0]);
   close(pipe_two[1]);

   input_file = fopen(fileB, "r");

  fprintf(stdout, "PAUSED\n");
   while (initial_pause) {
      ;
   }


   //The C library function char *fgets(char *str, int n, FILE *stream) reads a line from the 
   //specified stream and stores it into the string pointed to by str. 
   //input_file is the maximum number of characters to be read (including the final null-character and new line)
   //On success, the function returns the same str parameter. If the End-of-File is encountered
   //and no characters have been read, the contents of str remain unchanged and a null pointer is returned.
   //If an error occurs, a null pointer is returned.
   while(fgets(file_buffer, binary_length + 3, input_file) != NULL) {

      // remove newline and any carrage returns (windows)
      file_buffer[strcspn(file_buffer, "\r\n")] = 0;

      fprintf(stdout, "Complementer: Read binary string from FileB: %s     ", file_buffer);

      complement_processor(file_buffer, binary_length);

      fprintf(stdout, "Complementer: Write binary string to PIPE: %s\n", file_buffer);

      write(pipe_one[1], file_buffer, sizeof(file_buffer) + 1);

   }

   close(pipe_one[1]);

   signal (SIGINT, exit_handler);

   //pid_t waitpid(pid_t pid, int *stat_loc, int options);
   //The waitpid() function shall be equivalent to wait() if the pid argument is (pid_t)-1 and the options argument is 0. 
   //Otherwise, its behavior shall be modified by the values of the pid and options arguments.
   waitpid(-1, &status, 0);
   return 0;
}

//function that does the work of complementing binary numbers
//parameters: pointer to char array (binary number) & length of that binary #
//works by looping through every digit of the char array, starting from left,
//using the supplied length parameter and flipping 1s to 0s and 0s to 1s
void complement_processor(char *number, int binary_len) {

   int i;
   for (i = 0; i < binary_len; i++) {
      if (number[i] == '1') {
         number[i] = '0';
      } else {
         number[i] = '1';
      }
   }
}

//function that does the work of incrementing supplied binary number
//works by looping through digits of the char array,
//starting from right/last slot in array. if value is 0, just
//swap for 1 and break from the loop. if value is 1, turn the
//value 0 and keep looping until a position with 0 is found.
void increment_processor(char *number, int binary_len) {

   int i;
   for (i = binary_len - 1; i >= 0; i--) {
      if (number[i] == '0') {
         number[i] = '1';
         break;
      } else {
         number[i] = '0';
      }
   }
}



void add_processor(char *number_one, char *number_two, char *total, int binary_len){

   int carry = 0;

   int i;
   for (i = binary_len; i >= 0; i--) {

      //number_one[i] accounts for the binary number currently in our pipe
      //number_two[i] accounts for the binary number from fileA
      int digit_one = number_one[i] + '0';
      int digit_two = number_two[i]+ '0';

      int tmp_total = carry + digit_one + digit_two;

      //if number_one[i], number_two[i], and carry are 0, adding them will
      //result in 0 with no need for an additional carry bit
      if (tmp_total == 0 ) {
         carry = 0;

      //if one of either number_one[i], number_two[i], or the carry are 1 then
      //our total is 1, still no need for a carry bit
      } else if (tmp_total == 1) {
         number_one[i] = '1';
         carry = 0;

      //if two of number_one[i], number_two[i], or the carry are 1 then our total is 2,
      //we set our number_one[i] to 0 and carry out a 1 via the carry variable
      } else if (tmp_total == 2) {
         number_one[i] = '0';
         carry = 1;

      //if all 3 of number_one[i], number_two[i], and the carry are 1 then our total is 3.
      //we set our number_one[i] to 1 and carry out a 1 via the carry variable
      } else if (tmp_total == 3) {
         number_one[i] = '1';
         carry = 1;
      }
   }
}

void pause_handler (int sigNum) {
   if (initial_pause == 1) {
      fprintf (stdout, " Starting Processsing...\n");
      initial_pause = 0;
   }

}

void exit_handler (int sigNum) {
   printf(" exiting.\n");
   exit(0);
}	
