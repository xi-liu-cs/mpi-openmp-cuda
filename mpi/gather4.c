#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main()
{
    unsigned int my_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    
    unsigned int A = 2, B = 910, x = 91,
    n = B - A + 1, loc_n = n / comm_sz, 
    i, loc_a, loc_b, send_i, start, end;
    unsigned int * sendbuf = malloc(n * sizeof(unsigned int)), * recvbuf;
    if(!my_rank)
        recvbuf = malloc(n * sizeof(unsigned int));

    loc_a = A + my_rank * loc_n;
    loc_b = loc_a + loc_n - 1;
    if(my_rank == comm_sz - 1)
    {
        loc_b = B;
        loc_n = loc_b - loc_a + 1;
    }

    /* find numbers divisible by x */
    send_i = 0;
    for(i = loc_a; i <= loc_b; ++i)
    {
        if(!(i % x))
        {
            start = i;
            break;
        }
    }

    printf("rk = %d\n", my_rank);
    for(i = start; i <= loc_b; i += x)
    {
        sendbuf[send_i++] = i;
        printf("send_i - 1 = %d, sendbuf = %d\n", send_i - 1, sendbuf[send_i - 1]);
    }

    unsigned int max_send_i;
    MPI_Allreduce(&send_i, &max_send_i, 1, 
    MPI_UNSIGNED, MPI_MAX, MPI_COMM_WORLD);
    printf("send_i = %d, max_send_i = %d\n", send_i, max_send_i);

    MPI_Gather(sendbuf, send_i, MPI_UNSIGNED, recvbuf,
    max_send_i, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    for(i = 0; i < n; ++i)
        if(recvbuf[i] && recvbuf[i] % x == 0)
        printf("%d\n", recvbuf[i]);
    MPI_Finalize(); 
}