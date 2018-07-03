#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCK_SIZE 64
#define MEGA_BYTE (1024 * 1024)
#define MAX_LEN 1024

void *blockList[MAX_LEN];
long memTotal = 0;

int freeBlockList(int last) {
  for (int i = 0; i < last; i++) free(blockList[i]);
  printf("\nRam size approximates: %ldMB\n", memTotal);
  return 0;
}

void touchBlockList(int last) {
  for (int i = 0; i < last; i++)
    memset(blockList[i], 0, BLOCK_SIZE * MEGA_BYTE);
}

int main() {
  clock_t start, end;
  double timeThisRun = 0, timeLastRun = 0;

  for (int i = 0; i < MAX_LEN; i++) {
    if (!(blockList[i] = malloc(BLOCK_SIZE * MEGA_BYTE)))
      return freeBlockList(i);

    memTotal += BLOCK_SIZE;

    start = clock();
    touchBlockList(i);
    end = clock();

    timeLastRun = timeThisRun;
    timeThisRun = (double)(end - start) / CLOCKS_PER_SEC;

    printf("last run: %.2fs, this run: %.2fs, %ldMB touched\n", timeLastRun,
           timeThisRun, memTotal);

    if (memTotal >= 512 && timeLastRun > 0 && timeLastRun * 2 < timeThisRun)
      return freeBlockList(i);
  }

  return freeBlockList(MAX_LEN);
}
