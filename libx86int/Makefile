obj = idt.o idtdescriptor.o lidt.o interruptcontroller.o picdriver.o gdtdescriptor.o

CXX = i686-elf-g++
CC = i686-elf-g++
AR = i686-elf-ar
CPPFLAGS += -Iinclude/ -I/usr/cross/include/ -O0 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive -c

libx86idt: $(addprefix src/, $(obj))
	mkdir -p bin
	$(AR) rcs bin/libx86idt.a $(addprefix src/, $(obj))

install:
	mkdir -p ~/.quark/include
	mkdir -p ~/.quark/lib
	cp include/*.h ~/.quark/include
	cp bin/*.a ~/.quark/lib
