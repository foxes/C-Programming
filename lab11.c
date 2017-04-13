#include <Windows.h>
#include <iostream>
#include <math.h>

using namespace std;

/* 
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * Lab 11
 * Memory Management under Windows
 * Write a Windows-based program that:
 * queries the system to determine the page size
 * allocates a large chunk of memory: 1M (220) bytes
 * queries the system to determine the State of the allocated memory
 * report the State in user-friendly terms (i.e. Committed, Reserved, Free)
 * de-allocates the memory
 * repeats the query to determine memory State
 */
int main(){

	//getting pagesize
	SYSTEM_INFO si;
	//given function prototype
	GetSystemInfo(&si);
	cout << "Page Size : " << si.dwPageSize << " B\n";

	//allocating memory with malloc
	int *mem;
	mem = (int*)malloc(pow(2.0, 20.0));
	mem = (int*)malloc(1000000);
	cout << "\nAllocated Memory : " << (pow(2.0, 20.0))/1000 << " KB\n";

	//getting the allocated memory's state
	MEMORY_BASIC_INFORMATION mbi;

	VirtualQuery(mem, &mbi, sizeof(mbi));

	if(mbi.State == MEM_FREE){
		cout << "\nMemory Free\n";
	}else if(mbi.State == MEM_COMMIT){
		cout << "\nMemory Committed\n";
	}else if(mbi.State == MEM_RESERVE){
		cout << "\nMemory Reserved\n";
	}

	//freeing the allocated memory
	cout << "\nfree(memory)\n";
	free(mem);

	//getting the de-allocated memory's state
	VirtualQuery(mem, &mbi, sizeof(mbi));

	if(mbi.State == MEM_FREE){
		cout << "\nMemory Free\n";
	}else if(mbi.State == MEM_COMMIT){
		cout << "\nMemory Committed\n";
	}else if(mbi.State == MEM_RESERVE){
		cout << "\nMemory Reserved\n";
	}


	getchar();

	return 0;
}
