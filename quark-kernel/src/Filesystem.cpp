#include "Filesystem.h"
#include "Kernel.h"
#include "ATAPIODriver.h"
#include "Console.h"

/*
 * The root filesystem is the only one the kernel should ever mount.
 * Right now, this is assumed to be 'hda1'. Therefore,
 * 
 * TODO: Support root filesystem located somewhere other than hda1
 * */
void Filesystem::mount()
{
	/*
	 * The format of these headers is implied by "magic numbers" in this 
	 * function. This code should be modified soon to eliminate any 
	 * magic numbers and more clearly show what is going on (without
	 * using comments).
	 * */
	void* gptHeader = Kernel::kernelHeap.allocate(512);
	ATAPIODriver::hda->read(1, 1, gptHeader);
	/*uint64_t gptSignature = *((uint64_t*) gptHeader);
	if(gptSignature != 0x5452415020494645)
	{
		kout << "[fs] Bad GPT signature: " << gptSignature << " (should be " << (uint64_t) 0x5452415020494645 << "); HACF\n";
		while(true);
	}*/
	gptHeader = Kernel::kernelHeap.free(gptHeader);
	
	void* partHeader = heap.allocate(512);
	ATAPIODriver::hda->read(2, 1, partHeader);
	lbaBase = ((uint64_t*) partHeader)[4];
	partHeader = heap.free(partHeader);
	
	kout << "[fs] Part 1 is at " << (uint32_t) lbaBase << "\n";
	
	void* efsHeader = Kernel::kernelHeap.allocate(512);
	ATAPIODriver::hda->read(lbaBase, 1, efsHeader);
	uint64_t lbaOffset = ((uint64_t*) efsHeader)[3];
	efsHeader = Kernel::kernelHeap.free(efsHeader);
	
	while(lbaOffset != 0)
	{
		void* fileDescriptor = Kernel::kernelHeap.allocate(512);
		ATAPIODriver::hda->read(lbaBase + lbaOffset, 1, fileDescriptor);
		lbaOffset = ((uint64_t*) fileDescriptor)[0];
		//kout << "[fs] File found: " << (const char*) (reinterpret_cast<uint32_t>(fileDescriptor) + 256) << "\n";
		File* file = new File((char*) (reinterpret_cast<uint32_t>(fileDescriptor) + 256), ((uint64_t*) fileDescriptor)[4], ((uint64_t*) fileDescriptor)[2]);
		files.insertAtEnd(file);
		fileDescriptor = Kernel::kernelHeap.free(fileDescriptor);
	}
}

File* Filesystem::getFileInfo(char* filename)
{
	kout << "[fs] File requested: " << (const char*) filename << "\n";
	for(Iterator i = files.getFirst(); i != files.end; i = files.next(i))
	{
		//kout << "[fs] Comparing names: " << (const char*) filename << " and " << (const char*) files.get(i)->filename << "\n";
		bool nameMatch = false;
		for(int j = 0; j < 256; j++)
		{
			if(filename[j] == 0 && files.get(i)->filename[j] == 0)
			{
				nameMatch = true;
				break;
			}
			else if(filename[j] == 0 || files.get(i)->filename[j] == 0)
			{
				
				break;
			}
			else if(filename[j] != files.get(i)->filename[j])
			{
				break;
			}
		}
		
		if(nameMatch)
		{
			return files.get(i);
		}
	}
	return (File*) 0;
}

bool Filesystem::loadFile(void* dest, char* filename)
{
	File* file = getFileInfo(filename);
	if(file != 0)
	{
		kout << "[fs] Loading file at offset " << (uint32_t) file->fileDiskOffset << ", absolute " << (uint32_t) (lbaBase + file->fileDiskOffset) << "\n";
		return ATAPIODriver::hda->read(lbaBase + file->fileDiskOffset + 1, file->fileSectors, dest);
	}
	else
	{
		return false;
	}
}
