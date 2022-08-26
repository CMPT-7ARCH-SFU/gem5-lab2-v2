#include "vector.h"
#include "../vector_dma_clstr_hw_defines.h"

// #ifdef DMA_MODE
// #include "gem5/dma_interface.h"
// #endif

void vadd(){
    TYPE    * m1     = (TYPE    *)MATRIX1;
    TYPE    * m2     = (TYPE    *)MATRIX2;
    TYPE    * prod     = (TYPE    *)MATRIX3;
    int i, j, k;
    int k_col, i_col;
    TYPE mult;
    #pragma clang loop unroll_count(8)
    for(i=0;i<N;i++) {
        prod[i]  = 4*(m1[i] + m2[i]);
    }
    return;
}
