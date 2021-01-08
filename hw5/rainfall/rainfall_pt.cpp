#include "landscape_pt.h"
#include <time.h>


struct timespec start_time, end_time;

double calc_time(struct timespec start, struct timespec end)
{
  double start_sec = (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
  double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;
  if (end_sec < start_sec)
  {
    return 0;
  }
  else
  {
    return end_sec - start_sec;
  }
}

void create_threads(int num_threads, Landscape &landscape)
{
  //std::thread** threads = new std::thread*[num_threads];
  std::thread threads[num_threads];
  for (int j = 0; j < landscape.steps; j++)
  {
    for (int i = 0; i < num_threads; i++)
    {
      threads[i] = std::thread(&Landscape::reset_change, &landscape, i);
    }
    for (int i = 0; i < num_threads; i++)
    {
      threads[i].join();
    }
    for (int i = 0; i < num_threads; i++)
    {
      threads[i] = std::thread(&Landscape::rain, &landscape, i);
    }
    for (int i = 0; i < num_threads; i++)
    {
      threads[i].join();
    }
    for (int i = 0; i < num_threads; i++)
    {
      threads[i] = std::thread(&Landscape::water_change, &landscape, i);
    }
    for (int i = 0; i < num_threads; i++)
    {
      threads[i].join();
    }
  }

  /* // seq
 for(int j = 0; j < landscape.steps; j++){
   for(int i = 0; i < num_threads; i++){
     landscape.reset_change(i);
   }
  for (int i = 0; i < num_threads; i++)
  {
    landscape.rain(i);
  }
  for (int i = 0; i < num_threads; i++){
  landscape.water_change(i);
  }
  */


while (!landscape.check_dry())
{

  
    for (int i = 0; i < num_threads; i++) {
    threads[i] = std::thread(&Landscape::reset_change, &landscape, i);
        
    }
    for (int i = 0; i < num_threads; i++) {
      threads[i].join();
    }
    for (int i = 0; i < num_threads; i++) {
    threads[i] = std::thread(&Landscape::drain, &landscape, i);
        
    }
    for (int i = 0; i < num_threads; i++) {
      threads[i].join();
    }
    for (int i = 0; i < num_threads; i++) {
    threads[i] = std::thread(&Landscape::water_change, &landscape, i);
        
    }
    for (int i = 0; i < num_threads; i++) {
      threads[i].join();
    }
    
   /* // seq
  for (int i = 0; i < num_threads; i++)
  {
    landscape.reset_change(i);
  }
  for (int i = 0; i < num_threads; i++)
  {
    landscape.drain(i);
  }
  for (int i = 0; i < num_threads; i++)
  {
    landscape.water_change(i);
  }
  */
  landscape.add_one_step();
}
}

int main(int argc, char *argv[])
{
  int n_thread = atoi(argv[1]);
  int steps = atoi(argv[2]);
  double absorb_rate = atof(argv[3]);
  int N = atoi(argv[4]);
  std::string path = std::string(argv[5]);

  Landscape landscape = Landscape(n_thread, steps, absorb_rate, N, path);
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  create_threads(n_thread, landscape);

  clock_gettime(CLOCK_MONOTONIC, &end_time);
  double time_elapsed = calc_time(start_time, end_time);
  std::cout << "Rainfall simulation took " << landscape.finished_steps << " time steps to complete" << std::endl;
  std::cout << "Runtime = " << time_elapsed / 1000000000 << " seconds" << std::endl << std::endl;
  std::cout << "The following grid shows the number of raindrops absorbed at each point:" << std::endl;
  landscape.printResult();
  return EXIT_SUCCESS;
}