quark_obj = entry.o main.o scheduler.o Process.o Scheduler.o Interrupts.o \
Syscalls.o Filesystem.o File.o ELF.o PageAllocator.o math.o paging.o \
PagingStructure.o PageTable.o AddressSpace.o exceptions.o

static_libs = libpio.a libx86idt.a libheap.a libkconsole.a libatapio.a
link_script = linker.ld

CXX = i686-elf-g++
CC = i686-elf-g++
CPPFLAGS += -I$(HOME)/.quark/include -O0 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive -g

crti_obj=src/crti.o
crtbegin_obj:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
crtend_obj:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
crtn_obj=src/crtn.o

object_files = $(crti_obj) $(crtbegin_obj) $(addprefix src/, $(quark_obj)) $(crtend_obj) $(crtn_obj)

LINKFLAGS = -T $(link_script) -ffreestanding -nostdlib -lgcc 

quark: $(crti_obj) $(crtbegin_obj) $(crtend_obj) $(crtn_obj) $(addprefix src/, $(quark_obj))
	mkdir -p bin
	$(CXX) -I$(HOME)/.quark/include $(LINKFLAGS) $(object_files) $(addprefix ~/.quark/lib/, $(static_libs)) -o bin/quark

install:
	mkdir -p $(HOME)/.quark/bin
	cp bin/quark $(HOME)/.quark
