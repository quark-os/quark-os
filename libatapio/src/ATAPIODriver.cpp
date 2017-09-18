#include "ATAPIODriver.h"
#include "pio.h"
#include "Heap.h"
#include "Console.h"

ATADisk* ATAPIODriver::hda;
ATADisk* ATAPIODriver::hdb;
ATADisk* ATAPIODriver::hdc;
ATADisk* ATAPIODriver::hdd;

void ATAPIODriver::initialize()
{
	uint8_t primaryBusStatus = inb(0x1F7);
	if(primaryBusStatus != 0xFF)
	{
		kout << "[ata] Primary bus status: " << (void*) primaryBusStatus << "\n";
		hda = new ATADisk(true, true);
		hdb = new ATADisk(true, false);
		hda->initialize();
		hdb->initialize();
	}
	else
	{
		kout << "[ata] Floating bus detected: aborting\n";
	}
}


