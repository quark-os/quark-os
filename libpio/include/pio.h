#ifndef __PIO_H
#define __PIO_H

extern "C"
{
	void outb(short port, char data);
	char inb(short port);
	void outw(short port, short data);
	short inw(short port);
	void io_wait();
}

#endif
