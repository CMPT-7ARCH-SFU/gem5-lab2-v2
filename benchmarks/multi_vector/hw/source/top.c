#include "../hw_defines.h"

// void top(TYPE* m1_addr, TYPE* m2_addr, TYPE* m3_addr) {

void top() {
  TYPE mat = 1;
//   // DmaCopy(mat1, mat1, sizeof(TYPE));

//   // // Define Device MMRs
//   // volatile uint8_t *V2Flags = (uint8_t *)V2;
//   // volatile uint8_t *_DmaFlags = (uint8_t *)(DMA);
//   // volatile uint64_t *_DmaRdAddr = (uint64_t *)(DMA + 1);
//   // volatile uint64_t *_DmaWrAddr = (uint64_t *)(DMA + 9);
//   // volatile uint32_t *_DmaCopyLen = (uint32_t *)(DMA + 17);
  volatile uint8_t *_DMAFlags =  (uint8_t*)0x10020000;
  volatile uint32_t *_DMARdAddr  = (uint32_t*)0x10020001;
  volatile uint32_t *_DMAWrAddr  = (uint32_t*)0x10020009;
  volatile uint32_t *_DMACopyLen  = (uint32_t*)0x10020011;

  // *V1Flags = 0x0;
  // *V2Flags = 0x0;
  // *DmaFlags = 0x0;
  // TYPE* m1_addr = 0x80c00000;
  // // Transfer Input Matrices
  // // Transfer M1
  // DmaCopy(M1ADDR, m1_addr, vector_size);
  // *_DMARdAddr = mat1;
  // *_DMAWrAddr = mat1;
  // *_DMACopyLen = 0;
  // // // Fence it
  // *_DMAFlags = 0;
  // while (*_DMAFlags != 0x0);
  // *_DMAFlags = DEV_INIT;
  // // Poll DMA for finish
  // while ((*_DMAFlags & DEV_INTR) != DEV_INTR);
  // // Reset DMA
  // *DmaFlags = 0x0;  
  
  
  // // Transfer M2
  // *DmaRdAddr = m2_addr;
  // *DmaWrAddr = M2ADDR;
  // *DmaCopyLen = vector_size;

  // // Fence it
  // while (*DmaFlags != 0x0)
  //   ;
  // *DmaFlags = DEV_INIT;
  // // Poll DMA for finish
  // while ((*DmaFlags & DEV_INTR) != DEV_INTR)
  //   ;
  // // Reset DMA
  // *DmaFlags = 0x0;

  // // Check if accelerator ready for kickstarting
  // while (*V1Flags != 0x0)
  //   ;

  // // Start the accelerated function
  // *V1Flags = DEV_INIT;
  // // // Poll function for finish
  // while ((*V1Flags & DEV_INTR) != DEV_INTR)
    ;
  // *V1Flags = 0x0;

  // // Invoke accelerator 2

  // // Transfer Input Matrices
  // // Transfer the output of V1 to V2.
  // *DmaRdAddr = M3ADDR;
  // *DmaWrAddr = M1ADDR_V2;
  // *DmaCopyLen = vector_size;
  // *DmaFlags = DEV_INIT;
  // // //Poll DMA for finish
  // while ((*DmaFlags & DEV_INTR) != DEV_INTR)
  //   ;

  // // //	Transfer M2
  // // *DmaRdAddr = M3ADDR;
  // // *DmaWrAddr = M2ADDR_V2;
  // // *DmaCopyLen = vector_size;
  // // *DmaFlags = DEV_INIT;
  // // //Poll DMA for finish
  // // while ((*DmaFlags & DEV_INTR) != DEV_INTR)
  // // 	;
  // while (*V2Flags != 0x0)
  //   ;

  // // Start the accelerated function
  // *V2Flags = DEV_INIT;
  // // Poll function for finish
  // while ((*V2Flags & DEV_INTR) != DEV_INTR)
  //   ;
  // *V2Flags = 0x0;

  // // Transfer M3
  // *DmaRdAddr = M3ADDR_V2;
  // *DmaWrAddr = m3_addr;
  // *DmaCopyLen = vector_size;
  // *DmaFlags = DEV_INIT;
  // // Poll DMA for finish
  // while ((*DmaFlags & DEV_INTR) != DEV_INTR)
  //   ;

  return;
}