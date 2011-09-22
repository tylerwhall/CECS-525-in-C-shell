MEMORY {
	ROM (rx)  : ORIGIN = 0x0000, LENGTH = 16k
	RAM (!rx) : ORIGIN = 0x4000, LENGTH = 4k
}
SECTIONS {
	.reset : {
		iv.o (.text);
	} >ROM
	.text : {
		* (.text*);
		* (.rodata*);
	} >ROM
	/* The C code needs to reference all initialized data at its RAM address,
	   but we want it to be writable.  This causes the .data section to
	   be physically located after the .text section in ROM, but referenced
	   at its RAM address.  Code must copy this data section to RAM at boot.
	 */
	.data :  {
		data_load = LOADADDR(.data);
		data_start = .;
		* (.data);
		data_end = .;
	} >RAM AT>ROM
}
