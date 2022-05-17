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

    int loc_buf[loc_buf_sz + my_rank];
    printf("rk = %d\n", my_rank);
    for(int i = 0; i < loc_buf_sz + my_rank; ++i)
    {
        loc_buf[i] = my_rank + i;
        printf("locb = %d\n", loc_buf[i]);
    }

    if(!my_rank)
        recvbuf = malloc(comm_sz * (loc_buf_sz + comm_sz) * sizeof(int));
    
    int sendcount = loc_buf_sz + my_rank, recvcount = loc_buf_sz + comm_sz;
    // printf("sendcount = %d, recvcount = %d\n", sendcount, recvcount);
    MPI_Gather(loc_buf, sendcount, MPI_INT, recvbuf, recvcount,
    MPI_INT, 0, MPI_COMM_WORLD);

    if(!my_rank)
    {
        for(int i = 0; i < comm_sz * (loc_buf_sz + comm_sz); ++i)
            printf("%d ", recvbuf[i]);
        printf("\n");

        int offset = 0;
        for(int i = 0; i < comm_sz; ++i)
        {
            for(int j = 0; j < loc_buf_sz + my_rank; ++j)
            {
                printf("%d ", recvbuf[offset]);
                ++offset;
            }
            offset += (comm_sz - my_rank);
        }
    }

    MPI_Finalize();
}