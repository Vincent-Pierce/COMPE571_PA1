/****************************************************************************************
 * Author: Vincent Pierce 
 * 
 * Description: COMPE571 Programming Assignment 1 -- Comparing sequential, Multithreading,
 * and Multitasking workloads. This project implements a summation of numbers between 0 
 * and N, using three different methods. Case 1 is a baseline sequential implementation.
 * Case 2 is a Multithreaded implementation using POSIX pthreads. Case 3 uses multitasking 
 * via the fork system call to spawn new processes and pipes for inter-process communication. 
 * 
 * Sequential Implementation below
 * *************************************************************************************/

/* FILE INCLUSIONS *********************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <semaphore.h>
#include <sys/wait.h>
    
/* Global Variables ********************************************************************/
const uint64_t N[3] =           {100000000, 1000000000, 10000000000};
const uint8_t NUM_THREADS[3] =  {2, 4, 8};
const uint8_t NUM_TASKS[3] =    {2, 4, 8};

/* Function Declarations ***************************************************************/
void Baseline(uint64_t N);
void Multithreading(uint64_t N, uint8_t NUM_THREADS);
void* threadWork(void* bounds);
void Multiprocessing(uint64_t N, uint8_t NUM_PROCESSES);
void doWork(uint64_t lower, uint64_t upper, int pipe);