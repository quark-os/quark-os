all:
	make -C libpio
	make -C libpio install
	make -C libx86int
	make -C libx86int install
	make -C libheap
	make -C libheap install
	make -C libkconsole
	make -C libkconsole install
	make -C libtable install
	make -C quark-kernel
