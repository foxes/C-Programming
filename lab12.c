#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <dirent.h> 


/* 
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * Lab 12
 * File System Interface: Information
 * write a program that implements a small subset of the functionality of the "ls" command.
 * In particular, your program should use the file and directory system calls presented in 
 * this lab to incorporate the functionality supplied by:
 * file size in bytes: ls -l  // gives the file size in bytes 
 * file inode #
 * ls -i  // gives the file inode #
 */
int main(int argc, char *argv[]){

	struct stat statBuf;
	struct passwd *pwd;
	struct group *grp;

    if(argc != 3){
        printf("usage: %s [-l/i] [filepath]\n", argv[0]);
		exit(1);
    }


	DIR *d;
	struct dirent *dir;

	//The opendir() function opens a directory stream corresponding to the directory named by the dirname argument. 
	//The directory stream is positioned at the first entry.
	//Upon successful completion, opendir() returns a pointer to an object of type DIR. 
	//Otherwise, a null pointer is returned and errno is set to indicate the error.
	d = opendir(argv[2]);

	if (d){

		//The readdir() function returns a pointer to a structure representing the directory 
		//entry at the current position in the directory stream specified by the argument dirp,
		//and positions the directory stream at the next entry. 
		//It returns a null pointer upon reaching the end of the directory stream.
		// Positions the pointer at the first file in the selected directory.
		//so what this is doing is, checking if there is any files in the directory, and if there is
		//it starts going through them checking various read/write permissions and modes
		while ((dir = readdir(d)) != NULL){

			//copying 3rd arguement to a char array, SHOULD be the filepath that the
			//user wants to use, but theres no safety measures to prevent them from
                        //typing whatever they want currently
			char str[300];
			strcpy(str, argv[2]);

			// Copies the filename to the end of the directory path
			//targeting the first file in the directory
			strcat(str, dir->d_name);

			//The stat() function shall obtain information about the named file and 
			//write it to the area pointed to by the buf argument
			//returns -1 if error
			if(stat (str, &statBuf) < 0) {
				perror ("Error statBuf");
				exit(1);
			} 

			// if the command is -l
			if(strcmp(argv[1], "-l") == 0){

				//Checks if this is a directory
				if(S_ISDIR(statBuf.st_mode)){
					printf("d");
				}else{
					printf("-");
				}

				//checking for Read permission bit for the owner of the file
				if(statBuf.st_mode & S_IRUSR){
					printf("r");
				}else{
					printf("-");
				}

				//checking for Write permission bit for the owner of the file
				if(statBuf.st_mode & S_IWUSR){
					printf("w");
				}else{
					printf("-");
				}

				//checking for Execute (for ordinary files) or 
				//search (for directories) permission bit for the owner of the file.
				if(statBuf.st_mode & S_IXUSR){
					printf("x");
				}else{
					printf("-");
				}

				//checking for Read permission bit for the group owner of the file.
				if(statBuf.st_mode & S_IRGRP){
					printf("r");
				}else{
					printf("-");
				}

				//checking for Write permission bit for the group owner of the file
				if(statBuf.st_mode & S_IWGRP){
					printf("w");
				}else{
					printf("-");
				}

				//checking for Permission bit for the group owner of the file.
				if(statBuf.st_mode & S_IXGRP){
					printf("x");
				}else{
					printf("-");
				}

				//checking for Read permission bit for other users
				if(statBuf.st_mode & S_IROTH){
					printf("r");
				}else{
					printf("-");
				}

				//checking for Write permission bit for other users
				if(statBuf.st_mode & S_IWOTH){
					printf("w");
				}else{
					printf("-");
				}

				//checking for Execute or search permission bit for other users.
				if(statBuf.st_mode & S_IXOTH){
					printf("x");
				}else{
					printf("-");
				}

				//printing # of links and directories
				printf(" %ld", (long) statBuf.st_nlink);

				//Printing username of owner, if it exists
				if ((pwd = getpwuid(statBuf.st_uid)) != NULL)
					printf(" %s", pwd->pw_name);

				//Printing the group owner, if it exists
				if ((grp = getgrgid(statBuf.st_gid)) != NULL)
					printf(" %s", grp->gr_name);

				//Printing the file size
				//long long format expects a 64 bit value
				printf(" %lld", (long long) statBuf.st_size);

				//getting the date modified
				//this doesnt work....it just returns the current time
				//need to use  struct timespec st_mtim i think...but cant figure out how
				time_t timer;
				char buffer[26];
				struct tm* tm_info;

				time(&timer);
				tm_info = localtime(&timer);
				strftime(buffer, 26, "%b %d %H:%M", tm_info);
				printf(" %s", buffer);
				

				//Printing filename
				printf(" %s\n", dir->d_name);

			//ls -i is supposed to return the inode number for each file
			}else if(strcmp(argv[1], "-i") == 0){
				printf("%ld %s \n", (long) statBuf.st_ino, dir->d_name);
			}

		}

		closedir(d);
	}

	return 0;
}
