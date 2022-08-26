#include "vector.h"

void vadd(){
    TYPE    * m1     = (TYPE    *)MATRIX1;
    TYPE    * m2     = (TYPE    *)MATRIX2;
    TYPE    * prod     = (TYPE    *)MATRIX3;
    int i, j, k;
    #pragma clang loop unroll_count(8)
    for(i=0;i<N;i++) {
        prod[i]  = 4*(m1[i] + m2[i]);
    }
    return;
}
