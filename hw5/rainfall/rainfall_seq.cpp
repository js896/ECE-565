#include "landscape.h"
#include <time.h>

struct timespec start_time, end_time;

double calc_time(struct timespec start, struct timespec end) {
   double start_sec = (double)start.tv_sec*1000000000.0 + (double)start.tv_nsec;
   double end_sec = (double)end.tv_sec*1000000000.0 + (double)end.tv_nsec;
   if (end_sec < start_sec) {
     return 0;
   } else {
     return end_sec - start_sec;
   }
 }


int main(int argc, char* argv[]){
   int n_thread = atoi(argv[1]);
   int steps = atoi(argv[2]);
   double absorb_rate = atof(argv[3]);
   int N = atoi(argv[4]);
   std::string path = std::string(argv[5]);

   Landscape landscape = Landscape(steps, absorb_rate, N, path);
   clock_gettime(CLOCK_MONOTONIC, &start_time);
   landscape.simulate();
   clock_gettime(CLOCK_MONOTONIC, &end_time);
   double time_elapsed = calc_time(start_time, end_time);
   std::cout << "Rainfall simulation completed in " << landscape.finished_steps << " time steps " << std::endl;
   std::cout << "Runtime = " << time_elapsed/1000000000 << " seconds" << std::endl;
   landscape.printResult();
   return EXIT_SUCCESS;
}