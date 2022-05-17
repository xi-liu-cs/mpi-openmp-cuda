#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main()
{
    int my_rank, comm_sz, loc_i, loc_buf_sz = 3;
    int * recvbuf;
    /* buf_sz cannot be initialized as 0,
    then mpi_recv(buf, buf_sz) receive no element */
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    int loc_buf[loc_buf_sz];
    // printf("rk = %d\n", my_rank);
    for(int i = 0; i < loc_buf_sz; ++i)
    {
        loc_buf[i] = my_rank + i;
        // printf("locb = %d\n", loc_buf[i]);
    }

    if(!my_rank)
        recvbuf = malloc(comm_sz * loc_buf_sz * sizeof(int));
    
    MPI_Gather(loc_buf, loc_buf_sz, MPI_INT, recvbuf, loc_buf_sz,
    MPI_INT, 0, MPI_COMM_WORLD);

    if(!my_rank)
        for(int i = 0; i < comm_sz * loc_buf_sz; ++i)
            printf("%d ", recvbuf[i]);
    printf("\n");

    MPI_Finalize();
}