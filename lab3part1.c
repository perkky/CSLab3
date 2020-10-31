/* File:      lab3part1.c
 * Purpose:   Estimate pi using OpenMP and a monte carlo method
 * 
 * Compile:   gcc ...
 *            *needs my_rand.c, my_rand.h
 *
 * Run:       ./lab3part1 <number of threads> <number of tosses>
 *
 * Input:     None
 * Output:    Estimate of pi
 *
 * Note:      The estimated value of pi depends on both the number of threads and the number of "tosses".  
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "my_rand.h"

/*---------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   double pi_estimate;
   int thread_count;
   long long int number_in_circle;
   long long int number_of_tosses;
   
   thread_count = strtol(argv[1], NULL, 10);  
   number_of_tosses = strtoll(argv[2], NULL, 10);
   
   number_in_circle = 0;
   double startTime = omp_get_wtime();

   #pragma omp parallel num_threads(thread_count)   
   {
      int my_rank = omp_get_thread_num();
      unsigned seed = my_rank + 1;
      long long int toss;
      double x, y, distance_squared;

      #pragma omp for reduction(+: number_in_circle)
      for(toss = 0; toss < number_of_tosses; toss++) {
         x = 2*my_drand(&seed) - 1;
         y = 2*my_drand(&seed) - 1;
         distance_squared = x*x + y*y;
         if (distance_squared <= 1) number_in_circle++;
      }
   }

   printf("Time: %f\n", omp_get_wtime() - startTime);

   pi_estimate = 4*number_in_circle/((double) number_of_tosses);
   printf("Estimated pi: %e\n", pi_estimate);

   return 0;
}
