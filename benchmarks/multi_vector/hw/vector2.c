#include "hw_defines.h"
void top()
{
    uint8_t *m1base = (uint8_t *)V2_MAT1;
    uint8_t *m2base = (uint8_t *)V2_MAT2;
    uint8_t *m3base = (uint8_t *)V2_MAT3;
    TYPE *m1 = (TYPE *)m1base;
    TYPE *m2 = (TYPE *)m2base;
    TYPE *m3 = (TYPE *)m3base;
#pragma clang loop unroll(full)
    for (int k = 0; k < N; k++)
    {
        m3[k] = m1[k] * 8;
    }
}
