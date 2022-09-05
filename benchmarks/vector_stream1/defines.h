#ifndef __DEFINES_H__
#define __DEFINES_H__
#define CHECK
#include "vector_stream_clstr_hw_defines.h"
#include "../common/dma.h"
#define ROW 64
#define COL 64
#define CHECK

#define StreamIn  0x10020000 + 32
#define StreamOut 0x10020000 + 32
#define S3In S2Out
#define S1In StreamIn
#define S3Out StreamOut
#define S2In S1Out

#define TYPE uint32_t
#define vector_size N * sizeof(TYPE)

#define N 16

#endif
