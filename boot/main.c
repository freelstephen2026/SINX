CODE32

#include "asm.h"
#include "stdio.h"

void main(void)
{
	flush_screen();
	puts("LOAD loaded from FAT12 boot partition");
	printf("%c %d\n", 'a', -99);

	freeze();
}