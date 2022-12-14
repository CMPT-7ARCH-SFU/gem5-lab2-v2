#include "../../common/m5ops.h"
#include "../defines.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

volatile uint8_t *top = (uint8_t *)TOP;
volatile uint32_t *val_a = (uint32_t *)((uint64_t)TOP + 0x01);
volatile uint32_t *val_b = (uint32_t *)((uint64_t)TOP + 0x09);
volatile uint32_t *val_c = (uint32_t *)((uint64_t)TOP + 0x11);

int main(void) {
  m5_reset_stats();
  bool fail = false;
  // stage = 0;
  uint32_t base = 0x80c00000;
  TYPE *m1 = (TYPE *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
  TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);

  for (int i = 0; i < N; i++) {
    printf("%d", m1[i]);
  }

  *val_a = (uint32_t)m1;
  *val_b = (uint32_t)m2;
  *val_c = (uint32_t)m3;
  printf("%d\n", *top);
  *top = 0x01;
  int count;
  while (*top != 0)
    count++;
  printf("Job complete\n");
#ifdef CHECK
  printf("Checking result\n");
  for (int i = 0; i < N; i++) {
    if (m3[i] != 8 * (m1[i] + m2[i])) {
      fail = true;
      printf("Check failed\n");
      printf("Actual M3:%d\n", m3[i]);
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
