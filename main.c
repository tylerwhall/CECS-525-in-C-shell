#include "assembly.h"
#include "screen.h"
#include "kmem.h"
#include "string.h"

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

	/* Could be replaced with a call to memcpy */
	while (ram_data < &data_end) {
		*(ram_data++) = *(rom_data++);
	}
	if (foo != 0xdeadbeef)
		putstr("Warning: .data not initialized as expected\n");
}

void *test_malloc(int bytes) {
	void *ptr = kmalloc(bytes);
	putstr("Memory address: ");
	puthexint((int)ptr);
	putch('\n');
	return ptr;
}

void test_free(void *ptr) {
	putstr("Freeing: ");
	puthexint((int)ptr);
	putch('\n');
	kfree(ptr);
}

void memtest() {
	void *p1 = test_malloc(124);
	void __attribute__ ((unused)) *p2 = test_malloc(124);
	test_free(p1);
	void __attribute__ ((unused)) *p3 = test_malloc(124);

}

void shell() {
	char ptr[64];
	
	copy_data();
	putstr("Welcome to the (unstable) C Kernel\n");
	
	while(1)
	{
		putstr("> ");
		getstr(ptr, 64);
		if (strcmp(ptr, "STOP"))
		{
			putstr("Exiting...\n");
			return;
		}
		else if (strcmp(ptr, "MEMTEST"))
		{
			putstr("Performing Memory Test\n");
			memtest();
		}
		else if (strcmp(ptr, "ERRTEST"))
		{
			asm ("trap #0\n");
		}
		else
		{
			putstr("Echo: ");
			putstr(ptr);
			putch('\n');
		}
	}
}

void entry() {
	__asm_initialize__();
	kmeminit();
	
	shell();
	
	while(1);
}
