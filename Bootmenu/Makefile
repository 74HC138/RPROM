# Modified version of the m68k-bare-metal toolchain stand alone Makefile

# Sources in the source directory beside this Makefile.
SRCS=$(wildcard source/*.c source/*.S source/*.s)

# Specify the CPU type that you are targeting your build towards.
CPU=68000

# Set this to the directory containing libmetal.
TOOLCHAIN_PATH=./toolchain

# Uncomment either of the following depending on how you have installed gcc.
# PREFIX=m68k-linux-gnu
PREFIX=m68k-elf

BUILDDIR=build

CC=$(PREFIX)-gcc
LD=$(PREFIX)-ld
OBJCOPY=$(PREFIX)-objcopy
OBJDUMP=$(PREFIX)-objdump

CFLAGS=-m$(CPU) -Wall -Wextra -Wno-builtin-declaration-mismatch -g -static -Iinclude -I$(TOOLCHAIN_PATH)/include -nostdlib -msoft-float -MMD -MP -O
LFLAGS=--script=platform.ld -L$(TOOLCHAIN_PATH)/libmetal -lmetal-$(CPU)

OBJS=$(patsubst %.c,$(BUILDDIR)/%.c.o,$(SRCS))
OBJS:=$(patsubst %.S,$(BUILDDIR)/%.S.o,$(OBJS))
OBJS:=$(patsubst %.s,$(BUILDDIR)/%.s.o,$(OBJS))
DEPS=$(OBJS:.o=.d)

.PHONY: bmbinary release all crt clean rom dump dumps hexdump

bmbinary: crt $(OBJS)
	$(LD) -o $@ $(OBJS) $(LFLAGS)

release: CFLAGS+= -DNDEBUG
release: all

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.S.o: %.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.s.o: %.s
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)

all: bmbinary rom

crt: crt0.S
	$(CC) $(CFLAGS) -c -o crt0.o crt0.S
	rm -f crt0.d

clean:
	rm -rf $(BUILDDIR)
	rm -f bmbinary bmbinary.rom bmbinary.srec bmbinary* crt0.o crt0.d

rom:
	$(OBJCOPY) -O binary bmbinary bmbinary.rom
	$(OBJCOPY) -O srec bmbinary bmbinary.srec
	$(OBJCOPY) -I binary -O binary bmbinary.rom bmbinary.reverse.rom --reverse-bytes=2

dump:
	$(OBJDUMP) -mm68k:$(CPU) -belf32-m68k -st -j.evt bmbinary
	$(OBJDUMP) -mm68k:$(CPU) -belf32-m68k -dt -j.text bmbinary
	$(OBJDUMP) -mm68k:$(CPU) -belf32-m68k -st -j.rodata -j.data -j.bss -j.heap -j.stack bmbinary

dumps:
	$(OBJDUMP) -mm68k:$(CPU) -belf32-m68k -st -j.evt bmbinary
	$(OBJDUMP) -mm68k:$(CPU) -belf32-m68k -St -j.text bmbinary
	$(OBJDUMP) -mm68k:$(CPU) -belf32-m68k -st -j.rodata -j.data -j.bss -j.heap -j.stack bmbinary

hexdump:
	hexdump -C bmbinary.rom

emulator: bmbinary rom
	fs-uae --kickstart_file=bmbinary.rom --console_debugger=1