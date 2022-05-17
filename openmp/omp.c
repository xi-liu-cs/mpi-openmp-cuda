#include <stdio.h>
#include <omp.h>

void print()
{
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    printf("thread %d of %d\n", my_rank, thread_count);
}

int main()
{
    int n = 4;
    #pragma omp num_threads(n)
    #pragma omp parallel for
        for(int i = 0; i < n; ++i)
            print();
}