#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pwd.h>

using namespace std;


/**
   
   C++ program to work with interpreting unix man pages,
   using system calls and C library functions to access and report
   system information.

   @author Halston Raddatz
   CIS 452 - program1.cpp

 */
int main(void) {

    //for getting username of logged in user
    char *name;
    struct passwd *pass;
    pass = getpwuid(getuid());
    name = pass->pw_name;

    //for UID and GID, respectively
    uid_t uid = getuid();
    gid_t gid = getgid();

    //The setpwent() function rewinds to the beginning of the password database.
    struct passwd* p = NULL;
    setpwent();


    //for getting hostname of computer
    char hostname[1024];
    gethostname(hostname, 1024);


    cout << "User is: " << name << "\n";
    cout << "UID : " << uid << "\n";
    cout << "GID: " << gid << "\n";
    cout << "host is: " << hostname << "\n";

    //The getpwent() function returns a pointer to a
    // structure containing the broken-out fields of a record from the password database
    //The first time getpwent() is called, it returns the first entry; thereafter,
    // it returns successive entries.
    for (int i = 0; i < 5; i++) {
        if ((p = getpwent()) != NULL) {
            cout << "entry " << i << ": " << (*p).pw_name << "\n";
        }
    }
    //The endpwent() function is used to close
    //the password database after all processing has been performed.
    endpwent();

    //getting desired environment variable; example input: PATH
    string user_input;
    cout << "Environment variable desired: " << "\n";
    cin >> user_input;
    if (getenv(user_input.c_str()) == NULL ) {
        cout << "ERROR: variable not found \n";
    } else {
        cout << "value is: " << getenv(user_input.c_str()) << "\n";
    }


    return EXIT_SUCCESS;
}
