# mpi-openmp-cuda-project
# Xi Liu, 2022
# mpi: 
find numbers divisible by a given number x in the range 
between A and B (inclusive) and test scalability and performance. For example: find numbers 
divisible by 3 in the range between 2 and 10000.
# openmp: 
reads a series of p floating point numbers from a file then shows a 
histogram of n bins of these numbers. The output of your program is not a picture of a 
histogram just the values of each bin. Assume that the largest floating-point number will 
not be larger than 100.00 (one hundred). 
For example, suppose we have six numbers (37.7, 11.2, 17.7, 62.1, 72.4, 79.1) and four 
bins. This means: 
bin[0] will be responsible for numbers in the range [0, 25);
bin[1] will be responsible for numbers in the range [25, 50);
bin[2] will be responsible for numbers in the range [50, 75);
bin[3] will be responsible for numbers in the range [75, 100);
The output of your program on the screen must be: 
bin[0] = 2;
bin[1] = 1;
bin[2] = 2;
bin[3] = 1.
# cuda: 
user enters a number n (n is a positive integer). Three arrays, a, b, and c of n 
elements each are dynamically allocated and initialized with random floating points 
numbers. Then the operation c[i] += a[i] * b[i] will be executed both sequentially on the host and 
on the device (the GPU). The time taken by each are printed on the screen.
