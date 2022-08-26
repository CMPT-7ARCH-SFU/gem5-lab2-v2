#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"

volatile uint8_t  *ACC  = (uint8_t *)VECTOR_DMA;

inline void DmaCopy(void *dst, void *src, int len) {
  volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);
	//Transfer Input Features
  *DmaRdAddr  = (uint32_t)src;
	*DmaWrAddr  = (uint32_t)dst;
	*DmaCopyLen = len;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
}

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
  TYPE *spm1 = (TYPE *)spm_base;
  TYPE *spm2 = (TYPE *)((TYPE)spm_base + sizeof(TYPE) * N *2 );
  TYPE *spm3 = (TYPE *)((TYPE)spm_base + 2 * sizeof(TYPE) * N * 2);
#endif
  
  //Transfer Input Matrices
	//Transfer M1
  DmaCopy(spm1, m1, N * sizeof(TYPE));
	// Transfer M2
  DmaCopy(spm2, m2, N * sizeof(TYPE));

	// Start the accelerated function
	*ACC = DEV_INIT;
	//Poll function for finish
	while ((*ACC & DEV_INTR) != DEV_INTR);

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
