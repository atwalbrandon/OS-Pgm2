/****************************************************************************
    File: bsa190001_program2.c
    Author: Brandon Atwal
    Purpose: To create a program in C which solves the producer/consumer problem using semaphores and pthreads.
    Procedures:
		main:		- initialize semaphores and threads, join threads. Once done, destroy semaphores.
		producer:	- produce and append data. Output counts to excel file once time has elapsed
		consumer:	- takes the data from buffer and consumes it
*****************************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>

sem_t n, s;                 //semaphores for control
#define NUM_c 5 			//number of consumers
int sharedqueue[NUM_c];     //shared queue. global array implements circular queue
int times[10]; 				//The number of times of consumption

pthread_t pid1;  			//Producer threads id
pthread_t cth[NUM_c];		//Consumer threads
int pindex = 0;				//Producer index 
int cindex = 0;				//Consumer index 

int second = 300;			//The planned run time of the program in seconds (5 minutes)
time_t st, end;				//Start and end times of program




/**********************************************************************************
* int *producer()
* Author: Brandon Atwal
* Date: 03/11/2022
* Description: The producer will create (produce) the next long long value, which is
initialized to 0. It then sends that product to the buffer.
**********************************************************************************/
void *producer(void *arg)
{
    
	long long int next = 0; 	//initialize long long value to 0
    while (1) 
	{
		next = next + 1; 		//produce next long long value

		usleep(15000); 					//throttle for 15000 microseconds
		sem_wait(&s);

		sharedqueue[pindex] = next;		//append a product
		pindex = (pindex+1) % NUM_c;

		sem_post(&s);
		sem_post(&n);
    }
}

/**********************************************************************************
* int *consumer()
* Author: Brandon Atwal
* Date: 03/11/2022
* Description: The consumer takes data from the producer and consumes it.
It increments the count of times it has consumed.
**********************************************************************************/
void *consumer(void *arg)
{
    
    while (1) 
	{
		sem_wait(&n);
		sem_wait(&s);
		
		int data = sharedqueue[cindex]; 	//take data from producer
		sem_post(&s);

		//consume
		sharedqueue[cindex] = 0; 		//remove from sharedqueue
		cindex = (cindex+1) % NUM_c;  	//change index

        times[(int)arg]++; 				//increase the count of times this consumer has consumed
    }
}


/**********************************************************************************
* int main()
* Author: Brandon Atwal
* Date: 03/11/2022
* Description: Create the semaphores and pthreads. Once 5 minutes have elapsed,
it kills the threads and semaphores.
**********************************************************************************/
int main(int argc, char *argv[])
{
	
    st = time(NULL); 			//get the start time of the program

    sem_init(&s, 0, 5);			//Initialize the semaphores
    sem_init(&n, 0, 0);     

	//create then join pthreads
	for (int i = 0; i < NUM_c; i++) 					//create consumer threads
	{
    	pthread_create(cth + i, NULL, consumer, (void*)(i+1));
		printf("Consumer thread %d has started\n", i+1);
	}
    pthread_create(&pid1, NULL, producer, (void*)1); 	//create producer thread
	printf("Producer thread has started\n");

	//wait for specified time to elapse, then terminate threads
	while (1){								
		end = time(NULL); 					//get current time
	    if((end-st) >= second){ 			//if time elapsed is greater than runtime, end program
	    	FILE *fp = NULL;
			fp = fopen("output.xlsx", "w");
			fprintf(fp,"Consumer ID\tTimes Consumed\n"); 
			int i;
			for (i = 1; i <= NUM_c; i++) {	//output the counts
				fprintf(fp, "%d\t%d\n", i, times[i]);
			}
			fclose(fp); 					//close file

			for (int i = 0; i < NUM_c; i++) 
			{
				pthread_cancel(cth[i]); 	//cancel all consumer threads
			}
			pthread_cancel(pid1); 			//cancel the producer thread
			break;							//end loop
		}
	}

	/*to ensure threads are properly closed,
	  use join to wait until each is closed if not already*/
	for (int i = 0; i < NUM_c; i++){ 		//join consumer threads
		pthread_join(cth[i], NULL);
		printf("Consumer thread %d has ended\n", i+1);
	}
    pthread_join(pid1, NULL);				//join producer thread
	printf("Producer thread has ended\n");

    sem_destroy(&s); 						//destroy the semaphores
    sem_destroy(&n);

	printf("Program is complete. Counts outputted to output.xlsx\n");

    return 0;
}

