#ifndef __FILE_H
#define __FILE_H

#include <stdint.h>
#include <stddef.h>

class File
{
public:

	File();
	
	File(char* filename, uint64_t fileDiskOffset, uint64_t fileSectors);

	char filename[256];
	
	uint64_t fileDiskOffset;
	
	uint64_t fileSectors;

};

#endif
