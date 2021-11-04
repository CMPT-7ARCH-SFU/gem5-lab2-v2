#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(void) {
  m5_reset_stats();
  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;
  TYPE *m1 = (TYPE *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
  TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);
  TYPE *check = (TYPE *)(base + 3 * sizeof(TYPE) * N);
  common_val = 0;
  for (int i = 0; i < N; i++) {
    printf("m1: %p %ld\n", m1 + i, m1[i]);
  }

  for (int i = 0; i < N; i++) {
    printf("m2: %p %ld\n", m2 + i, m2[i]);
  }

  // Set up parameters for accelerators.
  // Base address of m1, Base address of m2, Base address of result.
  // All addresses are in CPU space
  arg1 = (uint64_t)base;
  arg2 = (uint64_t)(base + sizeof(TYPE) * N);
  arg3 = (uint64_t)(base + 2 * sizeof(TYPE) * N);

  // NO DMA required. As accelerator directly reaches into CPU space.

  printf("Acc Activated: %d\n", acc);
  acc = 0x01;
  while (acc != 0x0) {
    printf("Acc Status : %d\n", acc);
  }
  printf("Acc Done : %d\n", acc);

  acc = 0x00;

#ifdef CHECK
  bool fail = false;
  for (int i = 0; i < N; i++) {
      printf("M3  Actual : %ld\n", m3[i]);
    }

  if (fail)
    printf("Check Failed\n");
  else
    printf("Check Passed\n");
#endif
  m5_dump_stats();
  m5_exit();
}
