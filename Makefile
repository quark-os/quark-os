all:
	make -C libpio
	make -C libpio install
	make -C libx86int
	make -C libx86int install
	make -C quark-kernel
