#ifndef __DMA_H__
#define __DMA_H__

#define MMR_ADDR    0x2ff00000

#define STR_DMA_INIT_RD	0x01
#define STR_DMA_INIT_WR 0x02
#define STR_DMA_RD_INTR	0x10
#define STR_DMA_WR_INTR	0x20

#include "inttypes.h"
#include <stddef.h>

#define dType_Reg		int32_t
#define dType_8u		uint8_t
#define dType_16u		uint16_t
#define dType_32u		uint32_t
#define dType_8t		int8_t
#define dType_16t		int16_t
#define dType_32t		int32_t

#define MAX(a,b)		((a > b) ? a : b)
#define MIN(a,b)		((a < b) ? a : b)



#define MEM_DMA_INIT	0x01
#define MEM_DMA_INTR	0x04



inline void dmacpy(void * dst, void * src, int len) {
volatile char * FLAGS = (char *)(MMR_ADDR);
volatile size_t * SRC = (size_t *)(MMR_ADDR+1);
volatile size_t * DST = (size_t *)(MMR_ADDR+9);
volatile int * LEN = (int *)(MMR_ADDR+17);
    *SRC = (size_t)src;
    *DST = (size_t)dst;
    *LEN = len;
    *FLAGS |= 0x01;
}

inline int pollDma() {
    volatile char * FLAGS = (char *)(MMR_ADDR);
volatile size_t * SRC = (size_t *)(MMR_ADDR+1);
volatile size_t * DST = (size_t *)(MMR_ADDR+9);
volatile int * LEN = (int *)(MMR_ADDR+17);

    return ((*FLAGS&0x04)==0x04);
}
inline void resetDma() {
volatile char * FLAGS = (char *)(MMR_ADDR);
volatile size_t * SRC = (size_t *)(MMR_ADDR+1);
volatile size_t * DST = (size_t *)(MMR_ADDR+9);
volatile int * LEN = (int *)(MMR_ADDR+17);

    *FLAGS = 0;
}

#endif //__DMA_H__
