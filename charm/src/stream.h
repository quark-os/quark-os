#ifndef _STREAM_H
#define _STREAM_H

class Stream
{
public:
	virtual void operator<<(char* data) = 0;
	virtual void operator<<(uint8_t data) = 0;
	virtual void operator<<(uint16_t data) = 0;
	virtual void operator<<(uint32_t data) = 0;
	virtual void operator<<(uint64_t data) = 0;
};

#endif
