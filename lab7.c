/* 
 * Halston Raddatz & Bryana Craig
 * CIS 452
 * Greg Wolffe
 * lab 7
 * Filename: lab7.c
 */
#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

int main() {
  __int64 freq = 0;
  __int64 startCounter = 0;
  __int64 stopCounter = 0;
  __int64 start = 0;
  __int64 stop = 0;
  long double duration = 0;
  double period = 0;


  
  //The QueryPerformanceCounter() function retrieves the current value of the operating system's high-resolution counter. 
  //A non-zero return value indicates success, and the retrieved values will be in the specified variables.
  QueryPerformanceCounter((LARGE_INTEGER *)&startCounter);
  QueryPerformanceCounter((LARGE_INTEGER *)&start);
  QueryPerformanceCounter((LARGE_INTEGER *)&stopCounter);
    
  for(int i = 0; i < 1000000; i++);

  QueryPerformanceCounter((LARGE_INTEGER *)&stop);
  
  //The QueryPerformanceFrequency() function retrieves the frequency of the Windows 
  //operating system's high-resolution counter (i.e. number of clock_ticks/sec)
  //A non-zero return value indicates success, and the retrieved values will be in the specified variables.
  QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

  cout << "Frequency: " << freq << " ticks/second" << endl;

  __int64 counterDelay = stopCounter - startCounter;
  __int64 loopCounts  =(stop - start) - counterDelay;
  duration = ((double) loopCounts /(double) freq) * 1000.0;
  period = (1.0 / (double) freq) * 1000.0;

  cout << "Duration: " << duration << " milliseconds" << endl;
  cout << "Period: " << period << " milliseconds/tick" << endl;
  cout << "Counter (tick) overhead: " 
       << ((double) counterDelay / (double) freq)* 1000 
       << " milliseconds" << endl;

  Sleep(20000);

  return 0;
}
