charm_obj = charm.o entry.o pio.o idtdescriptor.o idt.o lidt.o interrupts.o window.o purevirtual.o interruptcontroller.o \
	picdriver.o isr.o interrupthandlers.o keyboard.o keyboardtranslator.o system.o display.o eventlistener.o console.o \
	ps2controller.o event.o eventkeyboard.o eventsystem.o allocatememory.o eventcommand.o
static_libs = libheap.a
link_script = linker.ld

CXX = i686-elf-g++
CC = i686-elf-g++
CPPFLAGS += -Iinclude/ -O0 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive 

crti_obj=src/crti.o
crtbegin_obj:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
crtend_obj:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
crtn_obj=src/crtn.o

object_files = $(crti_obj) $(crtbegin_obj) $(addprefix src/, $(charm_obj)) $(crtend_obj) $(crtn_obj)

LINKFLAGS = -T $(addprefix src/, $(link_script)) -ffreestanding -nostdlib -lgcc

charm: $(crti_obj) $(crtbegin_obj) $(crtend_obj) $(crtn_obj) $(addprefix src/, $(charm_obj))
	$(CXX) $(LINKFLAGS) $(object_files) $(addprefix lib/, $(static_libs)) -o bin/charm
