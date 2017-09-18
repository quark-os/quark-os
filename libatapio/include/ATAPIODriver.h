#ifndef __ATAPIODRIVER_H
#define __ATAPIODRIVER_H

#include "ATADisk.h"

class ATAPIODriver
{
public:
		
	static void initialize();
	
	static ATADisk* hda;
	static ATADisk* hdb;
	static ATADisk* hdc;
	static ATADisk* hdd;
	
};

#endif
