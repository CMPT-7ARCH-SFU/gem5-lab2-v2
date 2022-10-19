#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"

volatile uint8_t  *ACC  = (uint8_t *)VECTOR_DMA;

int main(void) {
  m5_reset_stats();
  uint64_t base = 0x80c00000;
  volatile TYPE *spm_base = (TYPE *)(MATRIX1);

  TYPE *m1 = (TYPE *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
  TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);
  TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*N);

  // Check that the gem5 script loaded the data in binary files into the appropriate location in memory.
  for (int i = 0; i < N; i++)
    printf("m1: %p %ld\n",m1+i,m1[i]);

  for (int i = 0; i < N; i++)
    printf("m2: %p %ld\n",m2+i,m2[i]);

#ifdef SPM
  TYPE *spm1 = (TYPE *)MATRIX1;
  TYPE *spm2 = (TYPE *)MATRIX2; //(TYPE *)((TYPE)spm_base + sizeof(TYPE) * N *2 );
  TYPE *spm3 = (TYPE *)MATRIX3; //(TYPE *)((TYPE)spm_base + 2 * sizeof(TYPE) * N * 2);
#endif
  TYPE* ARG1 = (TYPE*)( ACC + 0x1);
  TYPE* ARG2 = (TYPE*) (ACC + 0x09);
  TYPE* ARG3 = (TYPE*) (ACC + 0x11);

  *ARG1 = (TYPE)spm1;
  *ARG2 = (TYPE)spm2;
  *ARG3 = (TYPE)spm3;
  //Transfer Input Matrices
	//Transfer M1
  DmaCopy(spm1, m1, N * sizeof(TYPE));
	// Transfer M2
  DmaCopy(spm2, m2, N * sizeof(TYPE));

	// Start the accelerated function
	*ACC = DEV_INIT;
	while (*ACC != 0);

	// //Transfer M3
  DmaCopy(m3, spm3, N * sizeof(TYPE));

#ifdef CHECK
   bool fail = false;
   for (int i = 0; i < N; i++) {
     if (4*(m1[i] + m2[i]) != m3[i])
       {
       	printf("Expected : %ld  Actual : %ld\n",i, (4*(m1[i] + m2[i]),m3[i]));
 	      fail = true;
       }
     }

   if (fail)
     printf("Check Failed\n");
   else
    printf("Check Passed\n");
#endif
  //*(char *)0x2FFFFFFF = 1;
  m5_dump_stats();
  m5_exit();
}
