#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define file FILE
int block = 128;
float max = 100.0;
float range;
int b; /* number of bins */
clock_t start_time, end_time;
float tot_time;

int int_ceil(float a)
{
    int int_a = (int)a;
    if(a == (float)int_a)
        return int_a;
    else
        return int_a + 1;
}

void * alloc(size_t size)
{
    void * ret = malloc(size);
    if(!ret)
        printf("malloc error\n");
    return ret;
}

void * ralloc(void * ptr, size_t size)
{
    void * ret = realloc(ptr, size);
    if(!ret)
        printf("ralloc error\n");
    return ret;
}

/* store the entire file into an array of char pointers 
fp: file pointer
num_line: address of variable that would store number of lines */
char ** file_ary(file * fp, int * num_line)
{
    int cur_sz = block; 
    char ** lines = (char **)alloc(cur_sz * sizeof(char *));
    *num_line = 0;
    char * line = 0;
    size_t len = 0;
    ssize_t nread;
    while((nread = getline(&line, &len, fp)) != -1) 
    {
        if(*num_line >= cur_sz)
        {
            cur_sz += block;
            lines = (char **)ralloc(lines, cur_sz * sizeof(char *));
        }
        char * line_buf = (char *)alloc(nread * sizeof(char));
        strcpy(line_buf, line);
        lines[(*num_line)++] = line_buf;
    }
    return lines;
}

/* print the contents of the array of char pointers */
void print_lines(char ** lines, int num_line)
{
    for(int i = 0; i < num_line; ++i)
        printf("%s", lines[i]);
}

/* returns number of floating points 
the input file starts with an int, 
which is the number of floating points available */
int num_float(char ** lines, int num_line)
{
    for(int i = 0; i < num_line; ++i)
    {
        int cur_sz = block;
        char * ret = (char *)alloc(cur_sz * sizeof(int));
        int j = 0;
        int c;
        while((c = lines[i][j]) != ' ')
        {
            if(i >= cur_sz)
            {
                cur_sz += block;
                ret = (char *)ralloc(ret, cur_sz * sizeof(char));
            }
            ret[j++] = c;
        }
        ret[j] = '\0';
        if(j)
            return atoi(ret);
    }
    return 0;
}

/* convert the array of character pointers to array of numbers */
float * separate(char ** lines, int num_line, int num_float)
{
    float * num_ary = (float *)alloc((num_float + 1) * sizeof(float));
    int num_ary_i = 0;
    for(int i = 0; i < num_line; ++i)
    {
        if(num_ary_i >= num_float + 1)
            break;
        char * end_ptr = lines[i];
        while(num_ary_i < num_float + 1)
        {
            num_ary[num_ary_i++] = strtof(end_ptr, &end_ptr);
            if(isspace((int)end_ptr[1]))
                break;
        }
    }
    return num_ary;
}

/* print array of floats */
void print_floats(float * flt, int len)
{
    for(int i = 0; i < len; ++i)
        printf("%0.5f\n", flt[i]);
    printf("\n");
}

/* find the index of the number target within the bin[] array according to range:
bin[i] is responsible for numbers in the half-open interval
[i * range, i * range + range), 
if i * range <= target < i * range + range,
then the function returns i; 
if target is outside of the interval [0, max], return -1 */
int find_idx(float target, float range, float max)
{
    int i;
    for(i = 0; i < max; i += range)
    {
        if(i <= target && target < i + range)
            return int_ceil(i / range);
    }
    return -1;
}

/* fill the bin[] array with number of occurrences in each range 
flt: array of floating point numbers
n_f: number of floating point numbers
b: number of bins */
int * fill_bin(float * flt, int n_f)
{
    int * bin = (int *)alloc(b * sizeof(int));
    memset(bin, 0, b * sizeof(int));
    for(int i = 0; i < n_f; ++i)
    {
        float target = flt[i + 1]; /* i + 1 since *flt is number of floats */
        ++bin[find_idx(target, range, max)];
    }
    return bin;
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

    b = atoi(argv[1]);
    // int t = atoi(argv[2]);
    char * filename = argv[3];
    range = max / b;

    file * fp;
    if(!(fp = fopen(filename, "r")))
        printf("open error\n");

    int num_line = 0;
    char ** lines = file_ary(fp, &num_line);
    // print_lines(lines, num_line);

    int n_f = num_float(lines, num_line);
    // printf("\nnum_float = %d\n", n_f);

    float * flt = separate(lines, num_line, n_f);
    // print_floats(flt, n_f + 1);

    start_time = clock();
    int * bin = fill_bin(flt, n_f);
    end_time = clock();
    tot_time = (float)(end_time - start_time) / CLOCKS_PER_SEC;
    for(int i = 0; i < b; ++i)
        printf("b[%d] = %d\n", i, bin[i]);
    fclose(fp);
    printf("%f seconds\n", tot_time);
    return 0;
} 