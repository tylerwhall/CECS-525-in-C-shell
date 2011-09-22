#include "assembly.h"
#include "screen.h"

void entry() {
	char str[10];
	
	__asm_initialize__();
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
