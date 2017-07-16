bootstrap_src = bootstrap.asm fs.asm util.asm

.PHONY: x86-bios-bootstrap
x86-bios-bootstrap: #$(addprefix x86-bios-bootstrap/, $(bootstrap_src))
	mkdir -p bin
	nasm src/bootstrap.asm -o bin/x86-bios-bootstrap -O0
