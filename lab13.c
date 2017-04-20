

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* 
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * Lab 13
 * File System Interface: Operations
 * Implementing a program that mimics the ln command in *nix
 */

int main(int argc, char *argv[])
{    
	//-h = hard link, -s = soft link
	if(argc != 4){
        printf("enter parameters: -s/-h /pathtofile/ /pathoflink/ \n");
		exit(1);
    }

	//creating a hard link if arguement is -h
	if(strcmp(argv[1], "-h") == 0){

		link(argv[2], argv[3]);

	//creating a soft link if arguement is -s
	}else if(strcmp(argv[1], "-s") == 0){

		symlink(argv[2], argv[3]);

	}else{
		printf("bad arguement\n");
	}

	printf("\n");

	return 0;
}
