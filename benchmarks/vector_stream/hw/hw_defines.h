//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define STR_DMA_INIT_RD	0x01
#define STR_DMA_INIT_WR 0x02
#define STR_DMA_RD_INTR	0x10
#define STR_DMA_WR_INTR	0x20
#define STR_DMA_RD_RUNNING 0x04
#define STR_DMA_WR_RUNNING 0x08

#define DEV_INIT	0x01
#define DEV_INTR	0x04
#define MEM_DMA_INIT	0x01
#define MEM_DMA_INTR	0x04

#define VECTOR_SIZE		8

// StreamDMA
#define INPUT_SIZE		VECTOR_SIZE
#define OUTPUT_SIZE		VECTOR_SIZE

//Specify row/column sizes
#define row_size 	ROW
#define col_size 	COL
#define mat_size	row_size*col_size*sizeof(TYPE)