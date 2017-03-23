/* 
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * lab 9
 * example program to demonstrate memory management
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static char *data = "data";
static char *unData1;
static char *data2 = "data";
static char *unData2;
static char *data3 = "data";
static char *unData3;
static char *data4 = "data";
static char *unData4;
static char *data5 = "data";
static char *unData5;

int main (int argc, char *argv[]){

    int var = 0;
    int stack[1024];
    char * Heap = malloc(sizeof(char)*500);

    printf("env data =  %p\n", &argc);
    
    printf("Stack start = %p, Stack[0] = %p, Stack[99] = %p\n", &var, &stack[0], &stack[99]);

    printf("Heap start =  %p, Heap[0] =  %p,Heap[499] =  %p \n", sbrk(0), &Heap[0],&Heap[499]);

    printf("data1 = %p, unitialized data1 = %p\n", &data, &unData1);
    printf("data2 = %p, unitialized data2 = %p\n", &data2, &unData2);
    printf("data3 = %p, unitialized data3 = %p\n", &data3, &unData3);
    printf("data4 = %p, unitialized data4 = %p\n", &data4, &unData4);
    printf("data5 = %p, unitialized data5 = %p\n", &data5, &unData5);
 


    pause();

    return 0;
}
