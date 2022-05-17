#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

int main()
{
    /* mpicc scatter\ gather.c
    mpiexec -n 3 ./a */
    int my_rank, comm_sz;
    /* buf_sz cannot be initialized as 0,
    then mpi_recv(buf, buf_sz) receive no element */
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    int start = 2, end = 101, n = end - start + 1, loc_n = n / comm_sz, offset;
    size_t sz = comm_sz * sizeof(int);
    int * sendcounts = malloc(sz), * displacements = malloc(sz);
    offset = 0;
    for(int i = 0; i < comm_sz; ++i)
    {
        if((i == comm_sz - 1) && (n % comm_sz))
            loc_n = n / comm_sz + n % comm_sz;
        displacements[i] = offset;
        offset += loc_n;
        sendcounts[i] = loc_n;
    }

    /* scatterv */
    int * sendbuf, * recvbuf;

    if(!my_rank)
    {
        sendbuf = malloc(n * sizeof(int));
        for(int i = 0; i < n; ++i)
            sendbuf[i] = start + i;
    }
    printf("rank = %d, sendcounts[%d] = %d\n", my_rank, my_rank, sendcounts[my_rank]);
    recvbuf = malloc(sendcounts[my_rank] * sizeof(int));
    MPI_Scatterv(sendbuf, sendcounts, displacements, MPI_INT,
    recvbuf, sendcounts[my_rank], MPI_INT, 0, MPI_COMM_WORLD);
    for(int i = 0; i < sendcounts[my_rank]; ++i)
        printf("%d ", recvbuf[i]);
    printf("\n\n");

    /* gatherv */
    int * sendbuf2, * recvbuf2, * recvcounts2;

    if(!my_rank)
    {
        recvbuf2 = malloc(n * sizeof(int));
    }
    sendbuf2 = recvbuf;
    recvcounts2 = sendcounts;
    
    MPI_Gatherv(sendbuf2, sendcounts[my_rank], MPI_INT, recvbuf2,
    recvcounts2, displacements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Finalize();

    if(!my_rank)
    {
        printf("recvbuf2:\n");
        for(int i = 0; i < n; ++i)
            printf("%d ", recvbuf2[i]);
        printf("\n");
    }

    // if(!my_rank)
    // {
    //     printf("sendcounts:\n");
    //     for(int i = 0; i < comm_sz; ++i)
    //         printf("%d ", sendcounts[i]);
    //     printf("\n");
    // }
}