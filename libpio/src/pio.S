.intel_syntax noprefix

.global outb
outb: # outb(port, data)
	mov dx, [esp + 4]
	mov al, [esp + 8]
	out dx, al
	ret

.global inb
inb: # inb(port)
	mov dx, [esp + 4]
	in al, dx
	ret

.global outw
outw: # outw(port, data)
	mov dx, [esp + 4]
	mov ax, [esp + 8]
	out dx, ax
	ret

.global inw
inw: # inw(port)
	mov dx, [esp + 4]
	in ax, dx
	ret

.global io_wait
io_wait: # io_wait(); wastes an io cycle
	mov dx, 0x90
	mov al, 0
	out dx, al
	ret
