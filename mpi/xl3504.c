#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

// must compile with: mpicc -std=c99 -Wall -o checkdiv 

int main(int argc, char * argv[]){
  
unsigned int x, A, B;
unsigned int i; //loop index
FILE * fp; //for creating the output file
char filename[100] = ""; // the file name
// char * numbers; //the numbers in the range [2, N]

double start_p1, end_p1, start_p2, end_p2,
time_for_p1, loc_p1; 



/////////////////////////////////////////


start_p1 = clock();
// Check that the input from the user is correct.
if(argc != 4)
{
  printf("usage:  ./checkdiv A B x\n");
  printf("A: the lower bound of the range [A,B]\n");
  printf("B: the upper bound of the range [A,B]\n");
  printf("x: divisor\n");
  exit(1);
}  

A = (unsigned int)atoi(argv[1]); 
B = (unsigned int)atoi(argv[2]); 
x = (unsigned int)atoi(argv[3]);

unsigned int n = B - A + 1, loc_n, 
loc_a, loc_b, send_i = 0;
int start = -1, end = -1;
unsigned int * sendbuf, * recvbuf;
 

// The arguments to the main() function are available to all processes and no need to send them from process 0.
// Other processes must, after receiving the variables, calculate their own range.


/////////////////////////////////////////
int my_rank, comm_sz;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 


/////////////////////////////////////////
//start of part 1
start_p1 = MPI_Wtime();
// The main computation part starts here
loc_n = n / comm_sz;
loc_a = A + my_rank * loc_n;
loc_b = loc_a + loc_n - 1;
if(my_rank == comm_sz - 1)
{
  loc_b = B;
  loc_n = loc_b - loc_a + 1;
}

sendbuf = (unsigned int *)malloc((loc_n + 10) * sizeof(unsigned int));
if(!sendbuf)
  printf("cannot malloc\n");

for(i = loc_a; i <= loc_b; ++i)
{
  if(!(i % x))
  {
    start = i;
    break;
  }
}

if(start != -1)
{
  for(i = start; i <= loc_b; i += x)
  {
    sendbuf[send_i++] = i;
  }
}

unsigned int max_send_i;
MPI_Allreduce(&send_i, &max_send_i, 1, MPI_UNSIGNED, MPI_MAX, MPI_COMM_WORLD);

while(send_i < max_send_i)
    sendbuf[send_i++] = -1;

if(!my_rank)
{
  recvbuf = (unsigned int *)malloc((comm_sz * max_send_i + 10) * sizeof(unsigned int));
  if(!recvbuf)
    printf("cannot malloc\n");
}

MPI_Gather(sendbuf, send_i, MPI_UNSIGNED, recvbuf, max_send_i, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

// end of the main computation part
end_p1 = MPI_Wtime();
// Use reduction operation to get MAX of (end_p1 - start_p1) among processes 
// and send it to process 0 as time_for_p1
loc_p1 = end_p1 - start_p1;
MPI_Reduce(&loc_p1, &time_for_p1, 1,
MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
//end of part 1
/////////////////////////////////////////

if(!my_rank)
{
  //forming the filename
  start_p2 = MPI_Wtime();

  for(i = B; i >= A; --i)
  {
    if(!(i % x))
    {
        end = i;
        break;
    }
  }

  strcpy(filename, argv[2]);
  strcat(filename, ".txt");

  if(!(fp = fopen(filename,"w+t")))
  {
    printf("Cannot create file %s\n", filename);
    exit(1);
  }

  if(end != -1)
  {
    i = 0;
    for(; (i < comm_sz * max_send_i) && (recvbuf[i] != end); ++i) 
    {
        if(recvbuf[i] != -1)
            fprintf(fp, "%u\n", recvbuf[i]);
    }
    if(!i)
    {
        fprintf(fp, "%d\n", end);
    }
    else if(i - 1 >= 0)
    {
      if(recvbuf[i - 1] != end)
        fprintf(fp, "%d\n", end);
    }
  }

  fclose(fp);

  //Write the numbers divisible by x in the file as indicated in the lab description.
  end_p2 = MPI_Wtime();
//end of part 2
/////////////////////////////////////////

/* Print the times of the three parts */
  printf("time of part1 = %lf s    part2 = %lf s\n", 
        (double)(time_for_p1),
        (double)(end_p2-start_p2) );
}
MPI_Finalize();
return 0;
}
