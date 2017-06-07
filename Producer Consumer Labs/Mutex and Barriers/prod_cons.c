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

#define N 1000000//100000000

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

void producers(long size, double *warehouse)
{
	int i;
	/* Initialize the warehouse by random numbers. */
	srand(86456);
	double maxr = (double)RAND_MAX;
	int temp = rand();
	for(i = 0; i < size; i++)
		*(warehouse+i) = temp/maxr;
}

double consumers(long size, double *warehouse)
{
	int i;
	double sum = 0.0;
	for(i = 0; i < size; i++)
		sum += *(warehouse+i);
	return sum;
}

int main()
{
	double *a = (double *) malloc(N * sizeof(double));
	double sum = 0.0;
	int num_threads = 1;

	/* Call producers to generate stuff and call consumers to expend afterwards, and calculate the computation time. */
	start_timer();

	producers(N, a);
	sum = consumers(N, a);

	printf("With %d thread and %.4lf milliseconds, the sum is %.4lf.\n", num_threads, end_timer(), sum);

	return 0;
}
