#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include "List.h"
#include "File.h"

class Filesystem
{
public:

	void mount();
	
	File* getFileInfo(char* filename);
	
	bool loadFile(void* dest, char* filename);

private:

	uint64_t lbaBase;

	List<File*> files;

};

#endif
