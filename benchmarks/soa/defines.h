#ifndef __DEFINES_H__
#define __DEFINES_H__

#define common_val *(unsigned *)0x8f000000
#define acc *(char *)0x2f000000
#define arg1 *(int *)0x2f000001
#define arg2 *(int *)0x2f000009
#define arg3 *(int *)0x2f000011
#define rcIndex (r * ROW + c)

#define SPM
#define CHECK

#define TYPE uint32_t
#define N 16

#endif
