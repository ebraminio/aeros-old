#ifndef __DEVICES_PCI_IDE_H__
#define __DEVICES_PCI_IDE_H__

#include "devices/pci.h"
#include <stdbool.h>

typedef struct
{
	pci_device_t pci_device;
	struct
	{
		uint16_t base;
		uint16_t ctrl;
		uint16_t bus_master;
	} channels[2];
	bool irq_assign_req;
} ide_device_t;

// d device
// n ATA num from 1 to 4
// r registre
// v value
#define IDE_REG(d,n,r) (r+(r<=8?(n<3?d->channels[0]:d->channels[1]).base:(n<3?d->channels[0]:d->channels[1]).ctrl-8))
#define IDE_INB(d,n,r) inb(IDE_REG(d,n,r))
#define IDE_OUTB(d,n,r,v) outb(IDE_REG(d,n,r),v)
#define IDE_INW(d,n,r) inw(IDE_REG(d,n,r))
#define IDE_OUTW(d,n,r,v) outw(IDE_REG(d,n,r),v)


typedef enum
{
	ATA_DATA_REG,
	ATA_ERROR_REG,
	ATA_FEATURES_REG = 0x1,
	ATA_SECCOUNT0_REG,
	ATA_LBA0_REG,
	ATA_CYL0_REG,
	ATA_LBA1_REG = 0x4,
	ATA_CYL1_REG,
	ATA_LBA2_REG = 0x5,
	ATA_FLAGS_REG,
	ATA_STATUS_REG,
	ATA_CMD_REG = 0x7,
	ATA_SECCOUNT1_REG,
	ATA_LBA3_REG,
	ATA_LBA4_REG,
	ATA_LBA5_REG,
	ATA_CONTROL_REG,
	ATA_ALTSTATUS_REG = 0xC,
	ATA_DEVADDRESS_REG,
} ide_reg_t;

void ide_init(ide_device_t* device);

#endif