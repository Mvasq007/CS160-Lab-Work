/* mmm.c
 *
 *     Use a straightforward triple loop to perform a matrix-matrix multiplication.
 *
 *     Input:
 *         A[]: Matrix A to be multiplied.
 *         B[]: Matrix B to be multiplied.
 *         N:   Size of matrices A, B, and C. Suppose all matrices are square matrices for simplicity.
 *
 *     Output:
 *         C[]: Matrix C as a result of the multiplication.
 *         The time spent on doing the multiplication.
 *
 *     Usage:
 *         1. Compile: gcc -Wall mmm.c -o mmm
 *         2. Run:     ./mmm
 *
 *     Note:
 *         1. The matrices are stored in one-dimension arrays instead of two-dimension ones.
 *         2. The initial value of elements in the matrices are randomly generated before multiplication.
 *         3. As the memory is allocated for matrices on the stack and the default stack size may be not enough to keep all three matrices, if you would like to change the pre-defined matrix size N into a greater value. In this case, you have to increase the stack size for holding larger matrices. Please execute "ulimit -s unlimited" under the command line ("ulimit -s hard" on Mac OS).
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#define N 500

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

int main(int argc, char* argv[])
{
	double A[N*N], B[N*N], C[N*N];
	int i, j, k;
	int IsVerified = 1;

	/* Initialize matrices by random numbers. */
	srand(86456);
	double maxr = (double)RAND_MAX;
	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
		{
			if(!IsVerified)
			{
				/* Use simple numbers to verify the correctness of results. */
				A[i*N+j] = 1.0;
				B[i*N+j] = 1.0;
				C[i*N+j] = 0.0;
			}
			else
			{
				/* Actual scientific computation. */
				A[i*N+j] = rand()/maxr;
				B[i*N+j] = rand()/maxr;
				C[i*N+j] = rand()/maxr;
			}
		}
	int size,rank = 0;
	double val;
	MPI_Init(&argc, &argv);
    double store[N*N];
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, &size); /* get number of processes */
    
    int chunk = N/size;
   // if(N%size != 0)
    //{
	//	if(rank < N%size) chunk++;
	//}
	//start = 
	//printf("Chunk %d \n", chunk);
	MPI_Status* status = NULL;
	/* Do the multiplication and calculate the computation time. */
	start_timer();

	for(i = (rank*chunk); i < (rank+1)*chunk; i++)
		for(j = 0; j < N; j++)
			for(k = 0; k < N; k++)
				C[i*N+j] += A[i*N+k] * B[k*N+j];
				
	if(rank !=0)
	{
		 MPI_Send(C + rank*chunk*N, chunk*N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}
	
	
	if(rank == 0)
	{
		for(int m = 1; m<size; m++)
		{
			MPI_Recv(C+m*chunk*N, chunk*N, MPI_DOUBLE, m, 1, MPI_COMM_WORLD, status);
		}
	 }
	
				
	MPI_Finalize();
    if(rank == 0)
    {
	printf("Time for C(%d, %d) = C(%d, %d) + A(%d, %d) * B(%d, %d) is %.4lf milliseconds.\n", N, N, N, N, N, N, N, N, end_timer());
	printf("%d threads used.\n", size);

	/* Print out partial final results. */
	for(i = N-2; i < N; i++)
		for(j = N-2; j < N; j++)
			printf("%f\n", C[i*N+j]);
	}

	return 0;
}
