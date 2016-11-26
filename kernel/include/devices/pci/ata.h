#ifndef __DEVICES_PCI_ATA_H__
#define __DEVICES_PCI_ATA_H__

#include "devices/pci/ide.h"

void* atapio_read_sect(ide_device_t* device, uint8_t sata_num, unsigned int lba, uint8_t sector_count, void* buffer);
void atapio_write_sect(ide_device_t* device, uint8_t sata_num, unsigned int lba, uint8_t sector_count, void* buffer);

#endif