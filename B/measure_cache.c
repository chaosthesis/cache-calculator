#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/*
cache size: C
associativity: a
line size: b
delay penalty: D
number of sets: C/ab.
*/

/*
|Regime|Array Size |Stride         |Frequency of Misses    |Time per Iteration|
|------|-----------|---------------|-----------------------|------------------|
|1     |1 <= N <= C|1 <= s <= N/2  |no misses              |T_no_miss         |
|2.a   |C < N      |1 <= s <= b    |1 miss per b/s elements|T_no_miss + Ds/b  |
|2.b   |C < N      |b <= s <= N/a  |1 miss per element     |T_no_miss + D     |
|2.c   |C < N      |N/a <= s <= N/2|no misses              |T_no_miss         |
*/

#define K 1024
#define M 1024 * 1024
#define WORD_SIZE (sizeof(double))
#define MAX_CACHE_SIZE (M)
#define MAX_CACHE_LINE_SIZE (K)
#define MAX_ASSOCIATIVITY 256
#define MAX_ARRAY_SIZE (MAX_CACHE_SIZE / WORD_SIZE)
#define RUNS 11

double array[MAX_ARRAY_SIZE];
struct timeval t0, t1;

void init_array() {
  for (int i = 0; i < MAX_ARRAY_SIZE; i++) array[i] = (double)i / 2;
}

long get_elapsed() {
  return (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
}

void measure_cache_size() {
  printf("\n-> measuring cache size\n");
  int N = K / WORD_SIZE;
  int s = N / 8;
  long elapsed[K];
  int count;

  init_array();
  for (int run = 0; run < RUNS; run++) {
    N = K / WORD_SIZE;
    count = 0;
    while (N <= MAX_ARRAY_SIZE) {
      gettimeofday(&t0, 0);
      for (int i = 0; i < N; i += s) array[i] /= 2;
      gettimeofday(&t1, 0);
      elapsed[count++] += get_elapsed();
      N *= 2;
    }
  }

  count = 0;
  N = K / WORD_SIZE;
  while (N <= MAX_ARRAY_SIZE) {
    printf("vectorSize = %luKB, avgLAT = %ld\n", N * WORD_SIZE / K,
           elapsed[count++] / RUNS);
    N *= 2;
  }
}

void measure_cache_line_size() {
  printf("\n-> measuring cache line size\n");
  int N = MAX_ARRAY_SIZE;
  int s = 1;
  long elapsed[K];
  int count;

  init_array();
  for (int run = 0; run < RUNS; run++) {
    s = 1;
    count = 0;
    while (s * WORD_SIZE <= MAX_CACHE_LINE_SIZE) {
      gettimeofday(&t0, 0);
      for (int i = 0; i < N; i += s) array[i] /= 2;
      gettimeofday(&t1, 0);
      elapsed[count++] += get_elapsed();
      s *= 2;
    }
  }

  count = 0;
  s = 1;
  while (s * WORD_SIZE <= MAX_CACHE_LINE_SIZE) {
    printf("strideSize = %luB, avgLAT = %ld\n", s * WORD_SIZE,
           elapsed[count++] * s / RUNS);
    s *= 2;
  }
}

void measure_associativity() {
  printf("\n-> measuring associativity\n");
  int N = MAX_ARRAY_SIZE;
  int s = N / MAX_ASSOCIATIVITY;
  long elapsed[K];
  int count;

  init_array();
  for (int run = 0; run < RUNS; run++) {
    s = N / MAX_ASSOCIATIVITY;
    count = 0;
    while (s <= N / 2) {
      gettimeofday(&t0, 0);
      for (int i = 0; i < N; i += s) array[i] /= 2;
      gettimeofday(&t1, 0);
      elapsed[count++] += get_elapsed();
      s *= 2;
    }
  }

  count = 0;
  s = N / MAX_ASSOCIATIVITY;
  while (s <= N / 2) {
    printf("strideSize = %luKB, avgLAT = %ld (N /s = %d)\n", s * WORD_SIZE / K,
           elapsed[count++] * s * MAX_ASSOCIATIVITY / N / RUNS, N / s);
    s *= 2;
  }
}

int main() {
  printf("-> maxArraySize = %dKB\n", MAX_CACHE_SIZE / K);
  measure_cache_size();
  measure_cache_line_size();
  measure_associativity();
  return 0;
}
