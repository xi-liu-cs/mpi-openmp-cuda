#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main()
{
    int my_rank, comm_sz, buf_sz = 20;
    /* buf_sz cannot be initialized as 0,
    then mpi_recv(buf, buf_sz) receive no element */
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    if(my_rank)
    {
        printf("rank = %d\n", my_rank);
        char a[buf_sz];
        sprintf(a, "message from %d\n", my_rank);
        MPI_Send(a, strlen(a) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
       printf("rank = %d\n", my_rank);
       char b[buf_sz];
       *b = '\0';
       printf("before recv, b = %s\n", b);
       for(int i = 1; i < comm_sz; ++i)
        MPI_Recv(b, buf_sz, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("after recv, b = %s\n", b);
    }
    MPI_Finalize();
}