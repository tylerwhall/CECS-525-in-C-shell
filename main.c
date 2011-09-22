#include "assembly.h"
#include "screen.h"

extern unsigned char data_load;
extern unsigned char data_start;
extern unsigned char data_end;

int foo = 0xdeadbeef;

/* Copy the .data section from ROM to RAM */
static void copy_data(void)
{
	unsigned char *rom_data = &data_load;
	unsigned char *ram_data = &data_start;

	if (foo == 0xdeadbeef)
		putstr("Warning: .data appears initialized before copying\n");

	while (ram_data < &data_end) {
		*(ram_data++) = *(rom_data++);
	}
	if (foo != 0xdeadbeef)
		putstr("Warning: .data not initialized as expected\n");
}

void entry() {
	char str[10];
	
	__asm_initialize__();
	copy_data();
	putstr("Hello ");
	putstr(" World!!!\n");
	asm ("trap #0\n");
	
	putstr("Entering Echo Mode\n");
	while(1)
	{
		getstr(str, 10);
		putstr("Echo: ");
		putstr(str);
		putch('\n');
	}
}
