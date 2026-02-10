/****************************************************************************************
 * Author: Kyle Lainez
 * 
 * Description: COMPE571 Programming Assignment 1 -- Comparing sequential, Multithreading,
 * and Multitasking workloads. This project implements a summation of numbers between 0 
 * and N, using three different methods. Case 1 is a baseline sequential implementation.
 * Case 2 is a Multithreaded implementation using POSIX pthreads. Case 3 uses multitasking 
 * via the fork system call to spawn new processes and pipes for inter-process communication. 
 * 
 * Multithreading Implementation below
 * *************************************************************************************/

/* FILE INCLUSIONS *********************************************************************/
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

/* Source Code *************************************************************************/
#define N 1000000000ll
#define NUM_THREADS 8

typedef struct{
    uint64_t start;
    uint64_t end;
} ThreadArgs;


void* ChildThread(void* args){
    ThreadArgs* data = (ThreadArgs*)args;
    uint64_t* sum = (uint64_t*)malloc(sizeof(uint64_t));
    *sum = 0;
    
    uint64_t local_sum = 0;
    for(uint64_t i = data->start; i < data->end; i++){
        local_sum += i;
    }
    *sum = local_sum;
    pthread_exit((void*) sum);
}

int main(){
    ThreadArgs args[NUM_THREADS];
    pthread_t thread_id[NUM_THREADS];
    uint64_t* thread_sum = NULL;
    uint64_t sum = 0;

    //Clock
    struct timespec start, end;
    double elapsed;

    
    clock_gettime(CLOCK_MONOTONIC, &start);          // Start Clock

    // Create Threads
    for(uint64_t i=0; i < NUM_THREADS; i++){
        args[i].start = i * (N/NUM_THREADS);
        args[i].end = (i+1) == NUM_THREADS ? N : ((i+1) * (N/NUM_THREADS));
        pthread_create(&thread_id[i], NULL, ChildThread, &args[i]);
    }


    for (uint64_t i = 0; i< NUM_THREADS; i++){
        pthread_join(thread_id[i], (void**)&thread_sum);
        sum += *(uint64_t*)thread_sum;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);              // End clock
    
    // Calculate time betwen start and end clock
    elapsed = end.tv_sec - start.tv_sec;
    elapsed += (end.tv_nsec - start.tv_nsec) / 1e9;


    printf("The total summation of 0 through %llu is %lu\n", N, sum);
    printf("The total time to perform the workload multithreaded was %f seconds\n", elapsed);

    return 0;
}
