#include "File.h"
#include "Console.h"

File::File()
{
	
}

File::File(char* filename, uint64_t fileDiskOffset, uint64_t fileSectors)
{
	for(int i = 0;;i++)
	{
		if(filename[i] == 0)
		{
			this->filename[i] = 0;
			break;
		}
		else
		{
			this->filename[i] = filename[i];
		}
	}
	this->fileDiskOffset = fileDiskOffset;
	this->fileSectors = fileSectors;
}
