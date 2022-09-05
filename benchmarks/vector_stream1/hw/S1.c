#include "hw_defines.h"


void top() {
	volatile dType_8u * STR_IN  	= (dType_8u *)(S1In);
	volatile dType_8u * BUFFER 		= (dType_8u *)(S1Buffer); 
	volatile dType_8u * STR_OUT		= (dType_8u *)(S1Out);

	
	for (dType_Reg i = 0; i < INPUT_SIZE; i++) {
			*STR_OUT = (*STR_IN) + BUFFER[i];
        }
}