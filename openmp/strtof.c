
/* convert string having
   floating point as its content
   using strtof function */
  
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
  
int main()
{
    // Character array to be parsed
    char array[] = "365.25 7.0 9.131";
    int num = 3;
  
    // end pointer
    char * pend = array;
  
    float f[128];
    int i = 0;
    while(*pend) // or i < num
        f[i++] = strtof(pend, &pend);
    
    printf("pend = %s, len = %d\n", pend, strlen(pend));

    for(int j = 0; j < i; ++j)
        printf("%.4f\n", f[j]);
}