#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char * argv[])
{
    int count = 0;
    FILE * fp;
    float num = 100.0;
    char filename[100] = "";
    
    if(argc != 2)
    {
        printf("usage: ./genfile num\n");
        printf("num: the number of floating points to be generated\n");
        exit(1);
    }

    strcpy(filename, argv[1]);
    strcat(filename, ".txt");
    count = atoi(argv[1]);

    srand((unsigned int)time(NULL));

    if(!(fp = fopen(filename,"w+t")))
    {
	    printf("cannot create the file %s\n", filename);
	    exit(1);
    }

    fprintf(fp,"%d ", count); 
    for (int i = 0; i < count; ++i)
        fprintf(fp, "%f ", ((float)rand()/(float)(RAND_MAX)) * num);
    
    printf("generated %d floating point numbers and stored in file %s\n", count, filename);

    fclose(fp);
    return 0;
}
