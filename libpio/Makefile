obj = pio.o

CXX = i686-elf-g++
CC = i686-elf-g++
AR = i686-elf-ar
CPPFLAGS += -Iinclude/ -O0 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive -c

libatapio: $(addprefix src/, $(obj))
	mkdir -p bin
	$(AR) rcs bin/libpio.a $(addprefix src/, $(obj))

install:
	mkdir -p ~/.quark/include
	mkdir -p ~/.quark/lib
	cp include/*.h ~/.quark/include
	cp bin/*.a ~/.quark/lib

