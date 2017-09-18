#ifndef __ATADISK_H
#define __ATADISK_H

#include <stddef.h>
#include <stdint.h>

#include "ATARegister.h"
#include "ATABus.h"
#include "ATACommand.h"

class ATADisk
{
public: 

	ATADisk();
	
	ATADisk(bool primaryBus, bool masterDrive);

	void initialize();
	
	bool exists();
	
	bool read(uint64_t lba, uint64_t sectorCount, void* destination);
	
	//void write(uint64_t lba, uint64_t sectorCount, void* source);		May not be needed; the kernel itself shouldn't write to the disk. 

private:

	bool drivePresent;

	bool masterDrive;
	
	ATABus bus;
	
	uint16_t identifyData[256];
		
	void delay();
	
	void writeByte(ATARegister reg, uint8_t data);
	
	void sendCommand(ATACommand cmd);

	uint8_t readByte(ATARegister reg);
	
	uint16_t readWord(ATARegister reg);
	
};

#endif
