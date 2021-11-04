#include "vector.h"

#ifdef DMA_MODE
#include "gem5/dma_interface.h"
#endif

void vadd(TYPE m1[N], TYPE m2[N], TYPE prod[N]) {
  int i, j, k;
  int k_col, i_col;
  TYPE mult;
  volatile TYPE sum;
  for (j = 0; j < N; j++) {
    sum = 0;
#pragma clang loop unroll(full)
    for (i = 0; i < N; i++) {
      sum += (m1[i] * m2[i]);
    }
    prod[j] = sum;
  }
}