#ifndef __DMA_H__
#define __DMA_H__
// #include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#define MMR_ADDR    0x2ff00000

// volatile char * FLAGS = (char *)(MMR_ADDR);
// volatile size_t * SRC = (size_t *)(MMR_ADDR+1);
// volatile size_t * DST = (size_t *)(MMR_ADDR+9);
// volatile int * LEN = (int *)(MMR_ADDR+17);
// volatile unsigned flag;

// #define DMAFlags  0x10020000
// #define DMARdAddr  0x10020001
// #define DMAWrAddr  0x10020009
// #define DMACopyLen  0x10020011
#define DEVINIT	0x01
#define DEVINTR	0x04
void DmaCopy(uint32_t *dst, uint32_t *src, int len) {

    volatile uint8_t  * DmaFlags   = (uint8_t  *)(0x10020000);
    volatile uint64_t * DmaRdAddr  = (uint64_t *)(0x10020001);
    volatile uint64_t * DmaWrAddr  = (uint64_t *)(0x10020009);
    volatile uint32_t * DmaCopyLen = (uint32_t *)(0x10020011);
	//Transfer Input Features
    *DmaRdAddr  = (uint32_t)src;
	*DmaWrAddr  = (uint32_t)dst;
	*DmaCopyLen = len;
	*DmaFlags   = DEVINIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEVINTR) != DEVINTR);
}


void DmaCopy_32(uint32_t *m2, uint32_t *m1, int len) {

  volatile uint8_t *_DMAFlags =  (uint8_t*)0x10020000;
  volatile uint32_t *_DMARdAddr  = (uint32_t*)0x10020001;
  volatile uint32_t *_DMAWrAddr  = (uint32_t*)0x10020009;
  volatile uint32_t *_DMACopyLen  = (uint32_t*)0x10020011;
	//Transfer Input Features
  *_DMARdAddr = (uint32_t)m1;
  *_DMAWrAddr = (uint32_t)m2;
  *_DMACopyLen = len;
  // // // Fence it
  *_DMAFlags = 0;
  while (*_DMAFlags != 0x0);
  *_DMAFlags = DEVINIT;
  // Poll DMA for finish
  while ((*_DMAFlags & DEVINTR) != DEVINTR);
  // // Reset DMA
  *_DMAFlags = 0x0;  
}
// void dmacpy(void * dst, void * src, int len) {
//     *SRC = (size_t)src;
//     *DST = (size_t)dst;
//     *LEN = len;
//     *FLAGS |= 0x01;
// }

// int pollDma() {
//     return ((*FLAGS&0x04)==0x04);
// }
// void resetDma() {
//     *FLAGS = 0;
// }

#endif //__DMA_H__
