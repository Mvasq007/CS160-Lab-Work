/* threads_pc.c
* 860924181
* Login: mvasq007
* Section: 021
* TA: Li Tan
* Email: mvasq007@cs.ucr.edu
* Lab 5 - Exercise 2
*/

/* prod_cons.c
 *
 *     A sequential producer/consumer program. One function produces an array of random values. A second function consumes that array and sums it.
 *
 *     Input:
 *         N: Size of array of random numbers.
 *         a: Starting address of array of random numbers.
 *
 *     Output:
 *         The sum of all random numbers.
 *         The time spent on doing the summation.
 *
 *     Usage:
 *         1. Compile: gcc -Wall prod_cons.c -o prod_cons
 *         2. Run:     ./prod_cons
 *
 *     Note:
 *         1. The execution of functions producers() and consumers() are sequential and ordered.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#define N 1000000
#define NUM_THREADS  2
double *a = (double *) malloc (N * sizeof(double));
double *warehouse = a;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barr;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
double sum =0.0;

/*
 * Define time variables and functions.
 */

static struct timeval start_time;

static void start_timer()
{
	gettimeofday(&start_time, NULL);
}

static double end_timer()
{
	struct timeval end_time;
	gettimeofday(&end_time, NULL);
	return (end_time.tv_sec-start_time.tv_sec)*1000 + (end_time.tv_usec-start_time.tv_usec)/1000.0;
}

void * producers(void* thread_id)
{
	long ID = (long) thread_id;
	int chunk = N/NUM_THREADS;
	srand(86456);
	double maxr = (double)RAND_MAX;
	double temp = rand();
	//int rc = pthread_barrier_wait(&barr);
	for(int i = (ID*chunk); i <= chunk*(ID+1); i++)
	{
		/* Initialize the warehouse by random numbers. */
		//pthread_mutex_lock (&mutexsum);  
		*(warehouse+i) = temp/maxr;
		//pthread_mutex_unlock (&mutexsum);  
	}
	//int rc = pthread_barrier_wait(&barr);

}


void* consumers(void * thread_ID)
{
	long ID = (long)thread_ID;
	int chunk = N/NUM_THREADS;
	for(int i = (ID*chunk); i < chunk*(ID+1); i++)
	{
		pthread_mutex_lock (&mutexsum);
		sum += *(warehouse+i);
		pthread_mutex_unlock (&mutexsum); 
	}
	//int rc = pthread_barrier_wait(&barr);
}

int main()
{
	void *status;
	int rc;
	int t =0;
	/* Call producers to generate stuff and call consumers to expend afterwards, and calculate the computation time. */
	
    if(pthread_barrier_init(&barr, NULL, 2))
    {
        printf("Could not create a barrier\n");
        return -1;
    }
	
	start_timer();

	pthread_t * thread = (pthread_t *)malloc(sizeof(pthread_t)*NUM_THREADS);
	for(t =0; t<NUM_THREADS; t++)
	{
		rc = pthread_create(&thread[t], NULL, producers, (void *)t);

	}
    //rc = pthread_barrier_wait(&barr);
	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(thread[i], &status);
   }
	for(t =0; t<NUM_THREADS; t++)
	{
		rc = pthread_create(&thread[t], NULL, consumers, (void *)t);
	}
    //rc = pthread_barrier_wait(&barr);
	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(thread[i], &status);
		//pthread_mutex_destroy(&mutexsum); 
		//pthread_barrier_destroy(&barr);
   }	
   
	printf("With %d thread and %.4lf milliseconds, the sum is %.4lf.\n", NUM_THREADS, end_timer(), sum);
	//printf("With %d thread and %.4lf milliseconds, the sum is %.4lf.\n", NUM_THREADS, end_timer(), sum*2);

	return 0;
}

