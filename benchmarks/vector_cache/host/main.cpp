#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"
volatile uint8_t  *ACC  = (uint8_t *)VECTOR_CACHE;

int main(void) {
  m5_reset_stats();
  uint64_t base = 0x80c00000;
  // uint64_t spm_base = MATRIX1;
  TYPE *m1 = (TYPE *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
  TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);
  TYPE *check = (TYPE *)(base + 3 * sizeof(TYPE) * N);
  // common_val = 0;
  for (int i = 0; i < N; i++) {
    printf("m1: %p %ld\n", m1 + i, m1[i]);
  }

  for (int i = 0; i < N; i++) {
    printf("m2: %p %ld\n", m2 + i, m2[i]);
  }

  // Set up parameters for accelerators.
  // Base address of m1, Base address of m2, Base address of result.
  // All addresses are in CPU space
  TYPE* arg1 = (TYPE*)(ACC + 0x1);
  TYPE* arg2 = (TYPE*)( (uint64_t )arg1 + 8);
  TYPE* arg3 = (TYPE*)( (uint64_t )arg2 + 8);

  *arg1 = (TYPE)m1;
  *arg2 = (TYPE)m2;
  *arg3 = (TYPE)m3;

  printf("arg1: %p, arg2: %p\n", arg1, arg2);

  // NO DMA required. As accelerator directly reaches into CPU space.
  printf("Acc Activated: %d\n", *ACC);
  *ACC = 0x01;
  while (*ACC != 0x0) {
    printf("*ACC Status : %d\n", *ACC);
  }
  printf("*ACC Done : %d\n", *ACC);

  *ACC = 0x00;

#ifdef CHECK
  bool fail = false;
  for (int i = 0; i < N; i++) {
    if (4 * (m1[i] + m2[i]) != m3[i]) {
      printf("Expected : %ld  Actual : %ld\n", m1[i] + m2[i], m3[i]);
      fail = true;
    }
  }

  if (fail)
    printf("Check Failed\n");
  else
    printf("Check Passed\n");
#endif
  m5_dump_stats();
  m5_exit();
}
