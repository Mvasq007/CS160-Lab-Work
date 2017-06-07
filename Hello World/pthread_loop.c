/* pthread_loop.c
* 860924181	
* Login: mvasq007
* Section: 001
* TA: Li Tan
* Email: mvasq007@ucr.edu
* Lab 4 - Exercise 2
*/


/* pi_loop.c
 *
 *     A sequential program to calculate an approximate value of pi. This program will numerically compute the integral of 4/(1+x*x) from 0 to 1, which equals to pi. The precision of pi depends on the number of steps taken.
 *
 *     Input:
 *         num_steps: Number of steps of computing the integral.
 *
 *     Output:
 *         The approximate value of pi.
 *         The time spent on calculating the integral.
 *         The number of steps of calculating the integral.
 *
 *     Usage:
 *         1. Compile: gcc -Wall pi_loop.c -o pi_loop
 *         2. Run:     ./pi_loop
 *
 *     Note:
 *         1. Variables num_steps and step should be placed as global variables. Otherwise the precision of results suffer.
 *         2. The precision of pi keeps the same until the number of steps reaches a certain threshold.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_THREADS 4
long num_steps = 1000000;


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

double pi = 0;
double psum =0.0;

void * com_pi( void* thread_ID)
{
	long ID = (long)thread_ID;
	double x, sum;
	double step;
	int chunk = num_steps/NUM_THREADS;
	step = 1.0 / (double) num_steps;
	
	for(int i = (ID*chunk)+1; i <= chunk*(ID+1); i++)
	{
		x = (i - 0.5) * step;
		sum = sum + 4.0 / ( 1.0 + x * x);
	}
    psum += sum;
	
}

int main()
{

	/* Do the integration and calculate the computation time. */
	void *status;
	double step;
	step = 1.0 / (double) num_steps;
	pthread_t * thread = (pthread_t *)malloc(sizeof(pthread_t)*NUM_THREADS);
	start_timer();
	int rc;
	int t;
	for(t =0; t<NUM_THREADS; t++)
	{
		rc = pthread_create(&thread[t], NULL, com_pi, (void *)t); 
		if (rc){ 
			printf("ERROR; return code from pthread_create() is %d\n", rc); 
		exit(-1); 
		} 
	}
	
	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(thread[i], &status);
    }
	
	pi = step*psum;



	printf("pi is %f in %.4lf milliseconds, with %ld steps and %d thread.\n", pi, end_timer(), num_steps, NUM_THREADS);

	return 0;
}

