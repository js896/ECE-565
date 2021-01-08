#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

int num_elements;
int num_traversals;
uint64_t *array;
uint64_t *array_for_independence;
double elapsed_ns;

double calc_time(struct timespec start, struct timespec end) {
  double start_sec = (double)start.tv_sec*1000000000.0 + (double)start.tv_nsec;
  double end_sec = (double)end.tv_sec*1000000000.0 + (double)end.tv_nsec;

  if (end_sec < start_sec) {
    return 0;
  } else {
    return end_sec - start_sec;
  }
}

void init_array() {
    for (int i = 0; i < num_elements; ++i) {
        array[i] = i;
        array_for_independence[i] = i + 1;
    }
}

void write_only(int num_traversals) {
    for (int j = 0; j < num_traversals; ++j) {
        for (int i = 0; i < num_elements; ++i) {
            array[i] = 1;
        }
    }
}

void one_read_one_write(int num_traversals) {
    for (int j = 0; j < num_traversals; ++j) {
        for (int i = 0; i < num_elements; ++ i) {
            array[i] = array_for_independence[i];
        }
    }
}

void two_read_one_write(int num_traversals) {
    for (int j = 0; j < num_traversals; ++j) {
        for (int i = 0; i < num_elements; ++ i) {
            array[i] = array[i] + array_for_independence[i];
        }
    }
}

int main(int argc, char *argv[]) {

  struct timespec start_time, end_time;

  num_elements = atoi(argv[1]);
  num_traversals = atoi(argv[2]);

  array = (uint64_t*)malloc(num_elements * sizeof(uint64_t));
  array_for_independence = (uint64_t*)malloc(num_elements * sizeof(uint64_t));  
  init_array();

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  write_only(num_traversals);
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  elapsed_ns = calc_time(start_time, end_time);
  printf("Write traffic only, time = %f\n", elapsed_ns);
  printf("Bandwidth = %f Gbps\n", (((double)num_elements*8*num_traversals)/(elapsed_ns)));
  printf("\n");

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  one_read_one_write(num_traversals);
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  elapsed_ns = calc_time(start_time, end_time);
  printf("1:1 read-to-write ratio, time = %f\n", elapsed_ns);
  printf("Bandwidth = %f Gbps\n", (((double)num_elements*8*num_traversals*2)/(elapsed_ns)));
  printf("\n");

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  two_read_one_write(num_traversals);
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  elapsed_ns = calc_time(start_time, end_time);
  printf("2:1 read-to-write ratio, time = %f\n", elapsed_ns);
  printf("Bandwidth = %f Gbps\n", (((double)num_elements*8*num_traversals*3)/(elapsed_ns)));
  printf("\n");

  free(array);
  free(array_for_independence);
}
