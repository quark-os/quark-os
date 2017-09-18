#include "ELF.h"
#include "Console.h"

#include <stdint.h>
#include <stddef.h>

ELF::ELF()
{
	
}

ELF::ELF(void* file)
{
	this->file = file;
}

bool ELF::load()
{
	uint32_t* data = static_cast<uint32_t*>(file);
	if(data[0] != 0x464c457f)
	{
		kout << "[loader] Bad ELF header: " << (void*) data[0] << "\n";
		return false;
	}
	
	entry = reinterpret_cast<void*>(data[6]);
	uint32_t* programHeader = reinterpret_cast<uint32_t*>(data[7] + reinterpret_cast<uint32_t>(file));
	uint16_t programHeaderCount = reinterpret_cast<uint16_t*>(data)[22];
	uint16_t programHeaderSize = reinterpret_cast<uint16_t*>(data)[21];
	kout << "[loader] Loading binary with " << programHeaderCount << " segments and entry at " << entry << "\n";
	for(uint32_t i = 0; i < programHeaderCount; i++)
	{
		uint32_t* programHeaderEntry = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(programHeader) + (i * programHeaderSize));
		if(programHeaderEntry[0] == 0)
		{
			continue;
		}
		else if(programHeaderEntry[0] == 1)
		{
			uint8_t* segment = reinterpret_cast<uint8_t*>(programHeaderEntry[2]);
			uint8_t* segmentData = reinterpret_cast<uint8_t*>(programHeaderEntry[1] + reinterpret_cast<uint32_t>(file));
			uint32_t segmentSize = programHeaderEntry[5];
			uint32_t segmentDataSize = programHeaderEntry[4];
			kout << "[loader] Building section of size " << segmentSize << " at " << (void*) segment << "\n";
			for(uint32_t i = 0; i < segmentSize; i++)
			{
				segment[i] = 0;
			}
			for(uint32_t i = 0; i < segmentDataSize; i++)
			{
				segment[i] = segmentData[i];
			}
		}
		else
		{
			kout << "[loader] Unsupported section ID " << programHeaderEntry[0] << "! HACF!\n";
			while(true);
		}
	}
	return true;
}

void* ELF::getEntryPoint()
{
	return entry;
}
