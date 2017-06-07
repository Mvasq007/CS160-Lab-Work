/* hello_pthread.c
* 860924181	
* Login: mvasq007
* Section: 001
* TA: Li Tan
* Email: mvasq007@ucr.edu
* Lab 4 - Exercise 1
*/



/* hello_world.c
 *
 *     The very first computer program in any programming languages.
 *
 *     Input:
 *         None.
 *
 *     Output:
 *         The most beautiful sentence in computer science: Hello, world!
 *
 *     Usage:
 *         1. Compile: gcc -Wall hello_world.c -o hello_world
 *         2. Run:     ./hello_world
 *
 *     Note:
 *         1. Make sure you put -Wall as your compilation option to know every warning of compiling your program.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 5

void *Hello(void *thread_ID)
{
	long ID = (long)thread_ID;
	printf("Hello world! %ld \n", ID);
	pthread_exit(NULL);
}

int main(void)
{

	pthread_t * thread = (pthread_t *)malloc(sizeof(pthread_t)*NUM_THREADS);
	int rc;
	int t;
	for(t =0; t<NUM_THREADS; t++)
	{
		printf("In main: creating thread %d\n", t); 
		rc = pthread_create(&thread[t], NULL, Hello, (void *)t); 
		if (rc){ 
			printf("ERROR; return code from pthread_create() is %d\n", rc); 
		exit(-1); 
		} 
	}
	pthread_exit(NULL);
	return 0;
}

 

