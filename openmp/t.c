#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define file FILE
int block = 128;

char * next_num(file * fp, int * offset)
{/* return the next number pointed by fp
    assuming numbers are separated by ' ' */
    int cur_sz = block;
    char * ret = (char *)malloc(cur_sz * sizeof(int));
    int i = 0;
    int c;
    while((c = fgetc(fp)) != ' ')
    {
        if(i >= cur_sz)
        {
            cur_sz += block;
            ret = (char *)realloc(ret, cur_sz * sizeof(char));
        }
        ret[i++] = c;
    }
    ret[i] = '\0';
    if(offset)
        *offset = i + 1;
    return ret;
}

int main(int argc, char ** argv)
{
    if(argc != 4)
    {
	    printf("usage: ./histogram b t filename\n");
	    printf("b: is the number of bins, 0 < b <= 50\n");
	    printf("t: number of threads, 0 < t <= 100\n");
        printf("filename: the name of the text file that contains the floating point numbers\n");
 	    exit(1);
    }
    int b = atoi(argv[1]),
    t = atoi(argv[2]);
    char * filename = argv[3];

    file * fp;
    if(!(fp = fopen(filename, "r")))
        printf("cannot open\n");

    int offset;
    const char delim[] = " ";
    char * num_float = next_num(fp, &offset);
    printf("str = %s\n", num_float);
    int num_f = atoi(num_float);
    printf("num_f = %d\n", num_f);
    float f_ary[num_f];
    file * walk = fp;
    for(int i = 0; i < num_f; ++i)
    {
        // printf("oset = %d\n", offset);
        fseek(walk, offset, SEEK_SET);
        char * num = next_num(walk, &offset);
        // printf("num = %s\n", num);
        f_ary[i] = strtof(num, NULL);
    }
    // for(int i = 0; i < num_f; ++i)
    // printf("%.3f\n", f_ary[i]);
    fclose(fp);
} 