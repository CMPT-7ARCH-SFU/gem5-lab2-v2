#include <stdio.h>
#include "../defines.h"

void isr(void)
{
	printf("Interrupt\n\r");
	acc = 0;
}
