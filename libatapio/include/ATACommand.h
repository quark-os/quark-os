#ifndef __ATACOMMAND_H
#define __ATACOMMAND_H

enum ATACommand
{
	ATA_IDENTIFY = 0xEC,
	ATA_READ_LBA28 = 0x20
};

#endif
