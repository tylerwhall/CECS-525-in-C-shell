CC=m68k-elf-gcc
LD=m68k-elf-ld
OBJDUMP=m68k-elf-objdump
AS=m68k-elf-as
OBJCOPY=m68k-elf-objcopy
QEMU=/opt/m68k/bin/qemu-system-m68k

CFLAGS=-nostdlib -nostartfiles -nodefaultlibs
CFLAGS+=-Wall -Werror
CFLAGS+=-m68000
CFLAGS+=-Os

SOURCES=assembly.c screen.c main.c exception.c kmem.c string.c
OBJECTS=$(subst .c,.o,$(SOURCES))

main: ${OBJECTS} iv.o linker.x
	${LD} ${OBJECTS} iv.o -o main -T linker.x -Map main.map
	cp main attach_gdb_to_this
	${OBJCOPY} -O srec main

string.o: string.h string.c
	${CC} ${CFLAGS} -c string.c -o string.o
disassemble: main
	${OBJDUMP} -D attach_gdb_to_this

iv.o: iv.asm
	${AS} -o iv.o iv.asm

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<
%.d: %.c
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

DEPENDS=$(subst .c,.d,$(SOURCES))
-include ${DEPENDS}

run: main
	${QEMU} -M cecs -nographic -kernel main -gdb tcp::1234

debug: main
	${QEMU} -M cecs -nographic -kernel main -S -gdb tcp::1234

clean:
	rm iv.o attach_gdb_to_this main main.map ${DEPENDS} ${OBJECTS}

