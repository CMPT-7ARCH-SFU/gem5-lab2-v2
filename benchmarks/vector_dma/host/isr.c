#include <stdio.h>
#include "../defines.h"
extern volatile uint8_t * ACC;

void isr(void)
{
	printf("Interrupt\n\r");
	*ACC = 0x00;
}
