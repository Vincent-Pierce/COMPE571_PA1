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
#include "PA1.h"

/* Source Code *************************************************************************/
int main()
{
    uint64_t upperLimit = N[0];
    Baseline(upperLimit);
    Multithreading(upperLimit, 2);
    return 0;
}

void* threadWork(void* bounds)
{
    uint64_t* sum = malloc(sizeof(uint64_t)); 
    uint64_t localSum = 0;
    uint64_t* pBoundary = (uint64_t*)bounds; // make it a pointer so you can get the upper and lower bounds
    printf("Array element passed in: %lu\n", pBoundary[0]);
    uint64_t lowerLimit = *pBoundary;
    uint64_t upperLimit = *(pBoundary++); 
    for(int i = lowerLimit; i < upperLimit; i++)
    {
        localSum += i;
    }
    *sum = localSum;
    pthread_exit((void*)sum);
}
void Multithreading(uint64_t N, uint8_t NUM_THREADS)
{
    clock_t start, end = 0;
    uint64_t sum = 0;
    uint64_t upperLimit = N;
    uint64_t workLoad = N / NUM_THREADS;

    pthread_t threads[8] = {0}; // TODO use dynamic memory create size NUM_THREADS
    int err = 0;
    uint64_t* ret_val = 0;
    uint64_t bounds[100] = {0}; // TODO use dynamic memory to initialize the array to NUM_THREADS
    
    start = clock(); // work begins

    for(int i = 0; i < NUM_THREADS; i++) // create a thread for each NUM_THREADS
    {
        bounds[i*2] = workLoad*i; 
        bounds[i*2+1] = workLoad*(i+1); 
        printf("bounds array at index %d is %lu\n", i, bounds[i]);
        err = pthread_create(&threads[i], NULL, threadWork, (void*)bounds+i);
        if(err)
        {
            printf("Bad thread\n");
            kill(0); // terminate all processes
        }
    }
    for(int i = 0; i < NUM_THREADS; i++)
    {
        // wait for the threads here in the order you created them
        pthread_join(threads[i], (void**)&ret_val);
        sum += *ret_val;
    }

    end = clock(); // work complete 

    // Calculate time betwen start and end clock
    double time_taken = (double)(end - start);
    time_taken = time_taken / (double)(CLOCKS_PER_SEC);

    printf("The total summation of 0 through %lu with %d threads is %lu\n", upperLimit, NUM_THREADS, sum);
    printf("The total time to perform the workload multithreading was %f seconds\n", time_taken);

}











void Baseline(uint64_t N)
{
    uint64_t sum = 0;
    uint64_t upperLimit = N;
    clock_t start, end = 0;
    
    start = clock();           // Start clock
    for(int i=0;i<upperLimit;i++)    
    {
        sum+=i;
    }
    end = clock();             // End clock
    
    // Calculate time betwen start and end clock
    double time_taken = (double)(end - start);
    time_taken = time_taken / (double)(CLOCKS_PER_SEC);

    printf("The total summation of 0 through %lu is %lu\n", upperLimit, sum);
    printf("The total time to perform the workload sequentially was %f seconds\n", time_taken);
    
}