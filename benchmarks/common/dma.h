#ifndef __DMA_H__
#define __DMA_H__
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#define MMR_ADDR    0x2ff00000

volatile char * FLAGS = (char *)(MMR_ADDR);
volatile size_t * SRC = (size_t *)(MMR_ADDR+1);
volatile size_t * DST = (size_t *)(MMR_ADDR+9);
volatile int * LEN = (int *)(MMR_ADDR+17);
volatile unsigned flag;

#define DMAFlags  0x10020000
#define DMARdAddr  0x10020001
#define DMAWrAddr  0x10020009
#define DMACopyLen  0x10020011
#define DEVINIT	0x01
#define DEVINTR	0x04
inline void DmaCopy(void *dst, void *src, int len) {
    volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMAFlags);
    volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMARdAddr);
    volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMAWrAddr);
    volatile uint32_t * DmaCopyLen = (uint32_t *)(DMACopyLen);
	//Transfer Input Features
    *DmaRdAddr  = (uint32_t)src;
	*DmaWrAddr  = (uint32_t)dst;
	*DmaCopyLen = len;
	*DmaFlags   = DEVINIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEVINTR) != DEVINTR);
}

void dmacpy(void * dst, void * src, int len) {
    *SRC = (size_t)src;
    *DST = (size_t)dst;
    *LEN = len;
    *FLAGS |= 0x01;
}

int pollDma() {
    return ((*FLAGS&0x04)==0x04);
}
void resetDma() {
    *FLAGS = 0;
}

#endif //__DMA_H__
