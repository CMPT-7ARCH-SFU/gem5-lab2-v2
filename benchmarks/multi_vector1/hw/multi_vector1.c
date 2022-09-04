#include "hw_defines.h"

void top(){
    // TYPE *m1base = (TYPE *)MATRIX1;
    // TYPE *m2base = (TYPE *)MATRIX2;
    // TYPE *m3base = (TYPE *)MATRIX3;
    TYPE *m1 = (TYPE *)MATRIX1;
    TYPE *m2 = (TYPE *)MATRIX2;
    TYPE *m3 = (TYPE *)MATRIX3;
    // #pragma clang loop unroll(full)
    for (int k = 0; k < N; k++)
    {
        m3[k] = m1[k] + m2[k];
    }
}