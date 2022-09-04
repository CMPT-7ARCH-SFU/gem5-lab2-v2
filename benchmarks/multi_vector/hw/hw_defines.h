//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define DEV_INIT 0x01
#define DEV_INTR 0x04

//Specify row/column sizes
#define vector_size N * sizeof(TYPE)

// Device MMR addresses
// #define TOP 0x2f000000
// #define V1  0x2f0000F0
// #define V2  0x2f000100
// #define DMA 0x2ff00000

// Specify the scratchpad addresses for variables
// #define SPM 0x100200c0
// // #define M1ADDR SPM + (vector_size * 0)
// // #define M2ADDR SPM + (vector_size * 1)
// #define M3ADDR SPM + (vector_size * 2)

// // Specify the scratchpad addresses for variables
// #define SPM_V2 0x2f200000
// #define M1ADDR_V2 SPM_V2 + (vector_size * 0)
// #define M2ADDR_V2 SPM_V2 + (vector_size * 1)
// #define M3ADDR_V2 SPM_V2 + (vector_size * 2)