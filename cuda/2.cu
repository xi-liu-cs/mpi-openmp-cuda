#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda.h>
#include <math.h>

#define RANGE 11.79

/*** TODO: insert the declaration of the kernel function below this line ***/
__global__ void vecGPU(float * ad, float * bd, float * cd, int work_per_th, int n);

/**** end of the kernel declaration ***/

/* allocate ad, bd, and cd in the device */
void alloc(float ** ad_ptr, float ** bd_ptr, float ** cd_ptr, int n)
{
	cudaMalloc(ad_ptr, n * sizeof(float));
	cudaMalloc(bd_ptr, n * sizeof(float));
	cudaMalloc(cd_ptr, n * sizeof(float));
}

/* send a, b, and c to the device */
void send(float * a, float * b, float * c, float * ad, float * bd, float * cd, int n)
{
	cudaMemcpy(ad, a, n * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(bd, b, n * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(cd, c, n * sizeof(float), cudaMemcpyHostToDevice);
}

void print(float * a, int n)
{
	for(int i = 0; i < n; ++i)
		printf("%f ", a[i]);
	printf("\n");
}

int main(int argc, char *argv[]){

	int n = 0; //number of elements in the arrays
	int i;  //loop index
	float *a, *b, *c; // The arrays that will be processed in the host.
	float *temp;  //array in host used in the sequential code.
	float *ad, *bd, *cd; //The arrays that will be processed in the device.
	clock_t start, end; // to meaure the time taken by a specific part of code
	
	if(argc != 2){
		printf("usage:  ./vectorprog n\n");
		printf("n = number of elements in each vector\n");
		exit(1);
		}
		
	n = atoi(argv[1]);
	printf("Each vector will have %d elements\n", n);
	 

	//Allocating the arrays in the host
	
	if( !(a = (float *)malloc(n*sizeof(float))) )
	{
	   printf("Error allocating array a\n");
	   exit(1);
	}
	
	if( !(b = (float *)malloc(n*sizeof(float))) )
	{
	   printf("Error allocating array b\n");
	   exit(1);
	}
	
	if( !(c = (float *)malloc(n*sizeof(float))) )
	{
	   printf("Error allocating array c\n");
	   exit(1);
	}
	
	if( !(temp = (float *)malloc(n*sizeof(float))) )
	{
	   printf("Error allocating array temp\n");
	   exit(1);
	}
	
	//Fill out the arrays with random numbers between 0 and RANGE;
	srand((unsigned int)time(NULL));
	for (i = 0; i < n;  i++){
        a[i] = ((float)rand()/(float)(RAND_MAX)) * RANGE;
		b[i] = ((float)rand()/(float)(RAND_MAX)) * RANGE;
		c[i] = ((float)rand()/(float)(RAND_MAX)) * RANGE;
		temp[i] = c[i]; //temp is just another copy of C
	}
	
    //The sequential part
	start = clock();
	for(i = 0; i < n; i++)
		temp[i] += a[i] * b[i];
	end = clock();
	printf("Total time taken by the sequential part = %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    /******************  The start GPU part: Do not modify anything in main() above this line  ************/
	//The GPU part
	start = clock();
	
	/* TODO: in this part you need to do the following:
		1. allocate ad, bd, and cd in the device
		2. send a, b, and c to the device
		3. write the kernel, call it: vecGPU
		4. Call the kernel (the kernel itself will be written at the comment at the end of this file), 
		   you need to write the number of threads, blocks, etc and their geometry.
		5. Bring the cd array back from the device and store it in c array (declared earlier in main)
		6. free ad, bd, and cd
	*/
	alloc(&ad, &bd, &cd, n); /* 1 */
	send(a, b, c, ad, bd, cd, n); /* 2 */
	// int th_per_blk = 256, /* thread_per_block */
	// blk_per_grid = (n + th_per_blk - 1) / th_per_blk; /* block_per_grid = ceil(n / thread_per_block)*/
	// th_per_blk = 500;
	// blk_per_grid = 4; /* adjust numbers for report */
	// vecGPU<<<blk_per_grid, th_per_blk>>>(ad, bd, cd, n); /* 4 */

	int th_per_blk = 500, /* thread_per_block */
	blk_per_grid = 4, /* block_per_grid */
	work_per_th = n / (blk_per_grid * th_per_blk);
	// vecGPU<<<blk_per_grid, th_per_blk>>>(ad, bd, cd, work_per_th, n);
	vecGPU<<<1, 1>>>(ad, bd, cd, n, n);
	cudaMemcpy(c, cd, n * sizeof(float), cudaMemcpyDeviceToHost); /* 5 */
	cudaFree(ad); cudaFree(bd);	cudaFree(cd); /* 6 */
	end = clock();
	printf("Total time taken by the GPU part = %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	/******************  The end of the GPU part: Do not modify anything in main() below this line  ************/
	
	//checking the correctness of the GPU part
	for(i = 0; i < n; i++)
	  if( fabsf(temp[i] - c[i]) >= 0.009) //compare up to the second degit in floating point
		printf("Element %d in the result array does not match the sequential version\n", i);
		
	// Free the arrays in the host
	free(a); free(b); free(c); free(temp);

	return 0;
}


/**** TODO: Write the kernel itself below this line *****/ /* 3 */
__global__ void vecGPU(float * ad, float * bd, float * cd, int work_per_th, int n)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	for(int work = 0; work < work_per_th; ++work)
	{
		if(i < n)
		{
			cd[i] += ad[i] * bd[i];
			++i;
		}
	}
}