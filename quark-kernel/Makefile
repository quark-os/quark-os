quark_obj = entry.o main.o scheduler.o gdt.o
static_libs = libpio.a libx86idt.a
link_script = linker.ld

CXX = i686-elf-g++
CC = i686-elf-g++
CPPFLAGS += -I~/.quark/include/ -I/usr/cross/include -O0 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive -g

crti_obj=src/crti.o
crtbegin_obj:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
crtend_obj:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
crtn_obj=src/crtn.o

object_files = $(crti_obj) $(crtbegin_obj) $(addprefix src/, $(quark_obj)) $(crtend_obj) $(crtn_obj)

LINKFLAGS = -T $(link_script) -ffreestanding -nostdlib -lgcc

quark: $(crti_obj) $(crtbegin_obj) $(crtend_obj) $(crtn_obj) $(addprefix src/, $(quark_obj))
	mkdir -p bin
	$(CXX) $(LINKFLAGS) $(object_files) $(addprefix ~/.quark/lib/, $(static_libs)) -o bin/quark
