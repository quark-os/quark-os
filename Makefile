all:
	make -C libpio
	make -C libpio install
	make -C libx86int
	make -C libx86int install
	make -C libheap
	make -C libheap install
	make -C libkconsole
	make -C libkconsole install
	make -C libatapio
	make -C libatapio install
	make -C libtable install
	make -C x86-bios-bootstrap
	make -C quark-kernel

install:
	make -C quark-kernel install
	mkefs $(HOME)/.quark -b quark -o quark.img
	mksysimg quark.img -m x86-bios-bootstrap/bin/x86-bios-bootstrap -o quark.iso
	rm quark.img
