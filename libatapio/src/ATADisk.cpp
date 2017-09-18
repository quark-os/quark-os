#include "ATADisk.h"
#include "pio.h"
#include "Console.h"

#include <stddef.h>
#include <stdint.h>

ATADisk::ATADisk()
{
	this->drivePresent = false;
	this->bus = ATA_BUS_PRIMARY;
	this->masterDrive = true;
}

ATADisk::ATADisk(bool primaryBus, bool masterDrive)
{
	this->drivePresent = false;
	this->bus = primaryBus ? ATA_BUS_PRIMARY : ATA_BUS_SECONDARY;
	this->masterDrive = masterDrive;
}

void ATADisk::initialize()
{
	writeByte(ATA_DRIVE, masterDrive ? 0xA0 : 0xB0);
	writeByte(ATA_SECTOR_COUNT, 0);
	writeByte(ATA_LBA_LO, 0);
	writeByte(ATA_LBA_MID, 0);
	writeByte(ATA_LBA_HI, 0);
	sendCommand(ATA_IDENTIFY);
	delay();
	uint8_t status = readByte(ATA_STATUS);
	if(status == 0)
	{
		return;
	}
	else
	{
		while(((status = readByte(ATA_STATUS)) & 0b10000000) != 0);		// Waits until BSY is clear. Is there a better way to do this? This line of code looks odd.
		while(((status = readByte(ATA_STATUS)) & 0b00001000) == 0);		// TODO: Check for ERR bit set
		
		//TODO: Check for errors and correctly report them
		drivePresent = true;
		for(int i = 0; i < 256; i++)
		{
			identifyData[i] = readWord(ATA_DATA);
		}
	}
}

bool ATADisk::read(uint64_t lba, uint64_t sectorCount, void* destination)
{
	if(!drivePresent)
		return false;
	//kout << "[ata] Doing " << (uint32_t) sectorCount << " sector read at " << (uint32_t) lba << "\n";
	for(int i = 0; i < sectorCount; i += 256)
	{
		uint8_t readSize = sectorCount < 256 ? (uint8_t) sectorCount : 0;
		writeByte(ATA_DRIVE, masterDrive ? 0xE0 : 0xF0);
		writeByte(ATA_FEATURES, 0);
		writeByte(ATA_SECTOR_COUNT, (uint8_t) readSize);
		writeByte(ATA_LBA_LO, (uint8_t) lba);
		writeByte(ATA_LBA_MID, (uint8_t) (lba >> 8));
		writeByte(ATA_LBA_HI, (uint8_t) (lba >> 16));
		sendCommand(ATA_READ_LBA28);
		while((readByte(ATA_STATUS) & 0b10000000) != 0);	
		for(uint64_t j = 0; j < readSize; j++)
		{
			uint8_t status;
			while(((status = readByte(ATA_STATUS)) & 0b00001000) == 0)
			{
				if((status & 0b00000001) != 0)
				{
					kout << "[ata] Error during read. HACF.\n";
					while(true);
					return false;
				}
			}
			
			for(uint64_t k = 0; k < 256; k++)
			{
				uint16_t readData = readWord(ATA_DATA);
				reinterpret_cast<uint16_t*>(destination)[(j + i) * 256 + k] = readData;
			}
		}
	}
	return true;
}

bool ATADisk::exists()
{
	return drivePresent;
}

void ATADisk::delay()
{
	for(int i = 0; i < 4; i++)
	{
		readByte(ATA_STATUS);
	}
}

void ATADisk::writeByte(ATARegister reg, uint8_t data)
{
	outb(bus + reg, data);
}

void ATADisk::sendCommand(ATACommand cmd)
{
	writeByte(ATA_COMMAND, (char) cmd);
}

uint8_t ATADisk::readByte(ATARegister reg)
{
	return inb(bus + reg);
}

uint16_t ATADisk::readWord(ATARegister reg)
{
	return inw(bus + reg);
}
