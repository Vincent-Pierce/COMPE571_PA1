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
const uint64_t N = 1000000000;

/* Source Code *************************************************************************/
int main()
{
    uint64_t sum = 0;
    clock_t start, end;
    
    start = clock();           // Start clock
    for(uint64_t i=0;i<N;++i)
    {
        sum+=i;
    }
    end = clock();             // End clock
    
    // Calculate time betwen start and end clock
    double time_taken = (double)(end - start);
    time_taken = time_taken / (double)(CLOCKS_PER_SEC);

    printf("The total summation of 0 through %lu is %lu\n", N, sum);
    printf("The total time to perform the workload sequentially was %f seconds\n", time_taken);
    return 0;
}