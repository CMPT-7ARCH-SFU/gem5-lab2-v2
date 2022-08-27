#include "vector.h"

void vadd(TYPE m1[N], TYPE m2[N], TYPE prod[N] ){
    // TYPE *m1 = (TYPE *)0x80c00000;
    // TYPE *m2 = 0x80c00000 + sizeof(TYPE) * N;
    // TYPE *prod = 0x80c00000 + sizeof(TYPE) * N * 2;
    // m1[0] = m4[0];
    int i, j, k;
    int k_col, i_col;
    #pragma clang loop unroll_count(8)
    for(i=0;i<N;i++) {
            prod[i]  = 4*(m1[i] + m2[i]);
        
    }
}
