#include <stdio.h>
#include <mpi.h>

int main()
{
    int my_rank, comm_sz, loc_i;
    int recvbuf[comm_sz];
    /* buf_sz cannot be initialized as 0,
    then mpi_recv(buf, buf_sz) receive no element */
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    loc_i = my_rank * 2;
    MPI_Gather(&loc_i , 1, MPI_INT, recvbuf, 1,
    MPI_INT, 0, MPI_COMM_WORLD);

    if(!my_rank)
        for(int i = 0; i < comm_sz; ++i)
            printf("%d ", recvbuf[i]);
    printf("\n");

    MPI_Finalize();
}