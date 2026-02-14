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
    Multithreading(upperLimit, 8);
    Multiprocessing(upperLimit, 8);
    return 0;
}


void Multiprocessing(uint64_t N, uint8_t NUM_PROCESSES)
{
    struct timespec start, end;
    pid_t pid;
    pid_t* processes = malloc(sizeof(pid_t)*NUM_PROCESSES); // array of process ids 
    uint64_t sum, accumulator = 0;
    uint64_t workLoad = N / NUM_PROCESSES;
    int fd[2]; // 0 read 1 write 
    pipe(fd);

    clock_gettime(CLOCK_MONOTONIC, &start); // start work

    for(int i = 0; i<NUM_PROCESSES; i++) // spawn the processes
    {
        pid = fork();

        if(pid == 0)
        { // -------------- Child --------------
            close(fd[0]); // child only writes 
            doWork(workLoad*i, workLoad*(i+1), fd[1]); // child does work
            close(fd[1]);
            return; 
        } // -------------- Child --------------
        else
        {
            // close(fd[1]); // parent only reads
            processes[i] = pid; // parent registers child
        }
    }
    
    for(int i = 0; i<NUM_PROCESSES; i++)
    {
        wait(processes+sizeof(pid_t)*i); // any child termination will unblock
        size_t a = read(fd[0], &accumulator, sizeof(uint64_t));
        sum += accumulator;
    }
    close(fd[0]);

    clock_gettime(CLOCK_MONOTONIC, &end); // end clock

    // Calculate time betwen start and end clock
    double time_taken = end.tv_sec - start.tv_sec; 
    time_taken += (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\n\nThe total summation of 0 through %lu with %d processes is %lu\n", N, NUM_PROCESSES, sum);
    printf("The total time to perform the workload multiprocessing was %f seconds\n\n\n", time_taken);

    free(processes);
}
void doWork(uint64_t lower, uint64_t upper, int pipe)
{
    uint64_t sum = 0;
    int writePipe = dup(pipe);

    for(int i = lower; i < upper; i++)
    {
        sum +=i;
    }
    write(writePipe, &sum, sizeof(sum)); 
}

void* threadWork(void* bounds)
{
    uint64_t* sum = malloc(sizeof(uint64_t)); 
    uint64_t localSum = 0;
    uint64_t* pBoundary = (uint64_t*)bounds; // make it a pointer so you can get the upper and lower bounds
    uint64_t lowerLimit = pBoundary[0];
    uint64_t upperLimit = pBoundary[1]; 
    // printf("Lower Bound: %lu \n Upper Bound: %lu \n\n", lowerLimit, upperLimit);
    for(int i = lowerLimit; i < upperLimit; i++)
    {
        localSum += i;
    }
    *sum = localSum;
    pthread_exit((void*)sum);
}

void Multithreading(uint64_t N, uint8_t NUM_THREADS)
{
    struct timespec start, end;
    uint64_t sum = 0;
    uint64_t upperLimit = N;
    uint64_t workLoad = N / NUM_THREADS;

    pthread_t* threads = malloc(NUM_THREADS * sizeof(pthread_t));
    uint64_t* bounds = malloc((NUM_THREADS + 1)*sizeof(uint64_t));
    int err = 0;
    uint64_t* ret_val = 0;
    
    // start = clock(); // work begins
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i = 0; i < NUM_THREADS; i++) // create a thread for each NUM_THREADS
    {
        bounds[i] = workLoad*i; 
        bounds[i+1] = workLoad*(i+1); 
        // printf("bounds array at index %d is %lu\n, index %d is %lu\n\n", i, bounds[i], i+1, bounds[i+1]);
        err = pthread_create(&threads[i], NULL, threadWork, (void*) bounds + (i * sizeof(uint64_t)));
        if(err)
        {
            printf("Bad thread\n");
            kill(0, SIGKILL); // terminate all processes
        }
    }
    for(int i = 0; i < NUM_THREADS; i++)
    {
        // wait for the threads here in the order you created them
        pthread_join(threads[i], (void**)&ret_val);
        sum += *(uint64_t*)ret_val;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    // end = clock(); // work complete 

    // Calculate time betwen start and end clock
    double time_taken = end.tv_sec - start.tv_sec; 
    time_taken += (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("The total summation of 0 through %lu with %d threads is %lu\n", upperLimit, NUM_THREADS, sum);
    printf("The total time to perform the workload multithreading was %f seconds\n\n\n", time_taken);

    free(threads);
    free(bounds);

}

void Baseline(uint64_t N)
{
    uint64_t sum = 0;
    uint64_t upperLimit = N;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start); // start clock
    for(int i=0;i<upperLimit;i++)    
    {
        sum+=i;
    }

    clock_gettime(CLOCK_MONOTONIC, &end); // end clock

    // Calculate time betwen start and end clock
    double time_taken = end.tv_sec - start.tv_sec; 
    time_taken += (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("The total summation of 0 through %lu is %lu\n", upperLimit, sum);
    printf("The total time to perform the workload sequentially was %f seconds\n\n\n", time_taken);
    
}