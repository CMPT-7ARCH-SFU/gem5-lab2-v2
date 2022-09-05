#include "hw_defines.h"

volatile uint8_t *V1Flags = (uint8_t *)MULTI_VECTOR1;
volatile uint8_t *V2Flags = (uint8_t *)VECTOR2;

void top(TYPE m1[N], TYPE m2[N], TYPE m3[N]) {
  volatile uint8_t *_DMAFlags =  (uint8_t*)0x10020000;
  volatile uint32_t *_DMARdAddr  = (uint32_t*)0x10020001;
  volatile uint32_t *_DMAWrAddr  = (uint32_t*)0x10020009;
  volatile uint32_t *_DMACopyLen  = (uint32_t*)0x10020011;

  *_DMARdAddr = (uint32_t)m1;
  *_DMAWrAddr = (uint32_t)MATRIX1;
  *_DMACopyLen = vector_size;
  // // // Fence it
  *_DMAFlags = 0;
  while (*_DMAFlags != 0x0);
  *_DMAFlags = DEVINIT;
  // Poll DMA for finish
  while ((*_DMAFlags & DEVINTR) != DEVINTR);
  // // Reset DMA
  *_DMAFlags = 0x0;

  //Transfer m2 Features
  *_DMARdAddr = (uint32_t)m2;
  *_DMAWrAddr = (uint32_t)MATRIX2;
  *_DMACopyLen = vector_size;
  // // // Fence it
  *_DMAFlags = 0;
  while (*_DMAFlags != 0x0);
  *_DMAFlags = DEVINIT;
  // Poll DMA for finish
  while ((*_DMAFlags & DEVINTR) != DEVINTR);
  // // Reset DMA
  *_DMAFlags = 0x0;
  // DmaCopy_32(MATRIX2, m2, vector_size);
  // Check if accelerator ready for kickstarting
  while (*V1Flags != 0x0);

  // Start the accelerated function
  *V1Flags = DEV_INIT;
  // Poll function for finish
  while ((*V1Flags & DEV_INTR) != DEV_INTR);
  *V1Flags = 0x0;

  // // Invoke accelerator 2
  // Transfer Input Matrices
  // Transfer the output of V1 to V2.
  // DmaCopy_32(V2_MAT1, MATRIX3, vector_size);
  *_DMARdAddr = (uint32_t)MATRIX3;
  *_DMAWrAddr = (uint32_t)V2_MAT1;
  *_DMACopyLen = vector_size;
  // // // Fence it
  *_DMAFlags = 0;
  while (*_DMAFlags != 0x0);
  *_DMAFlags = DEVINIT;
  // Poll DMA for finish
  while ((*_DMAFlags & DEVINTR) != DEVINTR);
  // // Reset DMA
  *_DMAFlags = 0x0;
  
  // DmaCopy_32(V2_MAT2, MATRIX3, vector_size);
  *_DMARdAddr = (uint32_t)MATRIX3;
  *_DMAWrAddr = (uint32_t)V2_MAT2;
  *_DMACopyLen = vector_size;
  // // // Fence it
  *_DMAFlags = 0;
  while (*_DMAFlags != 0x0);
  *_DMAFlags = DEVINIT;
  // Poll DMA for finish
  while ((*_DMAFlags & DEVINTR) != DEVINTR);
  // // Reset DMA
  *_DMAFlags = 0x0;

  while (*V2Flags != 0x0);

  // Start the accelerated function
  *V2Flags = DEV_INIT;
  // Poll function for finish
  while ((*V2Flags & DEV_INTR) != DEV_INTR);
  *V2Flags = 0x0;
  
  // DmaCopy_32(m3, V2_MAT3, vector_size);
  *_DMARdAddr = (uint32_t)V2_MAT3;
  *_DMAWrAddr = (uint32_t)m3;
  *_DMACopyLen = vector_size;
  // // // Fence it
  *_DMAFlags = 0;
  while (*_DMAFlags != 0x0);
  *_DMAFlags = DEVINIT;
  // Poll DMA for finish
  while ((*_DMAFlags & DEVINTR) != DEVINTR);
  // // Reset DMA
  *_DMAFlags = 0x0;
  // return;
}