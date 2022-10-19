#include "hw_defines.h"

void top(uint64_t in_addr, uint64_t out_addr, uint64_t const_addr) {

  // Initialize Accelerators
  volatile uint8_t *s1 = (uint8_t *)S1;
  volatile uint8_t *s2 = (uint8_t *)S2;
  volatile uint8_t *s3 = (uint8_t *)S3;

  // Initialize DMAs
  // StreamDma
  volatile uint8_t *StrDmaFlags = (uint8_t *)(STREAMDMA_Flags);
  volatile uint64_t *StrDmaRdAddr = (uint64_t *)(STREAMDMA_Flags + 4);
  volatile uint64_t *StrDmaWrAddr = (uint64_t *)(STREAMDMA_Flags + 12);
  volatile uint32_t *StrDmaRdFrameSize = (uint32_t *)(STREAMDMA_Flags + 20);
  volatile uint8_t *StrDmaNumRdFrames = (uint8_t *)(STREAMDMA_Flags + 24);
  volatile uint8_t *StrDmaRdFrameBuffSize = (uint8_t *)(STREAMDMA_Flags + 25);
  volatile uint32_t *StrDmaWrFrameSize = (uint32_t *)(STREAMDMA_Flags + 26);
  volatile uint8_t *StrDmaNumWrFrames = (uint8_t *)(STREAMDMA_Flags + 30);
  volatile uint8_t *StrDmaWrFrameBuffSize = (uint8_t *)(STREAMDMA_Flags + 31);
  // MemDma
  volatile uint8_t *MemDmaFlags = (uint8_t *)(DMA_Flags);
  volatile uint64_t *MemDmaRdAddr = (uint64_t *)(DMA_Flags + 1);
  volatile uint64_t *MemDmaWrAddr = (uint64_t *)(DMA_Flags + 9);
  volatile uint32_t *MemDmaCopyLen = (uint32_t *)(DMA_Flags + 17);
  // Initialize DRAM-Stream DMA
  *StrDmaRdAddr = in_addr;
  *StrDmaRdFrameSize = INPUT_SIZE;
  *StrDmaNumRdFrames = 1;
  *StrDmaRdFrameBuffSize = 1;
  // Initialize Stream-DRAM DMA
  *StrDmaWrAddr = out_addr;
  *StrDmaWrFrameSize = OUTPUT_SIZE;
  *StrDmaNumWrFrames = 1;
  *StrDmaWrFrameBuffSize = 1;
  // Start Stream DMAs
  // *StrDmaFlags = STR_DMA_INIT_RD;
  *StrDmaFlags = STR_DMA_INIT_RD | STR_DMA_INIT_WR;

  // Transfer constants table
  *MemDmaRdAddr = const_addr;
  *MemDmaWrAddr = S1Buffer;
  *MemDmaCopyLen = INPUT_SIZE * 1;
  *MemDmaFlags = MEM_DMA_INIT;
  // Poll DMA for finish
  while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

  // Start S1
  *s1 = 0x01;
  // Start S2
  *s2 = 0x01;
  // // Start S3
  *s3 = 0x01;
  // Wait for all accelerators to finish before sending interrupt to CPU
  while ((*StrDmaFlags & 0x08) == 0x08)
    ;
    
  *s1 = 0x00;
  // Start S2
  *s2 = 0x00;
  // Start S3
  *s3 = 0x00;

  return;
}