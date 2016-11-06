#include "devices/pci/ata.h"
#include "devices/pci/ide.h"
#include <sys/io.h>
#include "io/log.h"

#define MASTER	0xE0
#define SLAVE	0xF0

// Status
#define ERR 1<<0	// Cleared by any command
#define DRQ 1<<3	// Drive is ready for PIO data communications (in or out)
#define SRV 1<<4	// Overlapped Mode Service Request
#define DF	1<<5	// Drive Fault Error (does not ser ERR)
#define RDY	1<<6	// Clear after an error or when drive is spun down
#define BSY	1<<7

#define READ_PIO		0x20
#define READ_PIO_EXT	0x24
#define READ_DMA		0xC8
#define READ_DMA_EXT	0x25
#define WRITE_PIO		0x30
#define WRITE_PIO_EXT	0x34
#define WRITE_DMA		0xCA
#define WRITE_DMA_EXT	0x35
#define CACHE_FLUSH		0xE7
#define CACHE_FLUSH_EXT	0xEA
#define PACKET			0xA0
#define IDENTIFY_PACKET	0xA1	// PATAPI devices only
#define IDENTIFY		0xEC
#define READ			0xA8
#define EJECT			0x1B

#define ATA_CMD(device, n, cmd) IDE_OUTB(device, n, ATA_CMD_REG, cmd)

static void ide_poll(ide_device_t* device, uint8_t sata_num)
{
	pci_read(&device->pci_device, 0);
	pci_read(&device->pci_device, 0);

	uint8_t status;
	while((status=IDE_INB(device, sata_num, ATA_STATUS_REG)) & BSY);

	if(status & ERR)
		panic("IDE error");
	if(status & DF)
		panic("IDE device fault");
	if((status & DRQ) == 0)
		panic("IDE not ready after operation");
}

static void ata_identify(ide_device_t* device, uint8_t sata_num)
{
	IDE_OUTB(device, sata_num, ATA_FLAGS_REG, 0xA0);
	IDE_OUTB(device, sata_num, ATA_SECCOUNT0_REG, 0);
	IDE_OUTB(device, sata_num, ATA_LBA0_REG, 0);
	IDE_OUTB(device, sata_num, ATA_LBA1_REG, 0);
	IDE_OUTB(device, sata_num, ATA_LBA2_REG, 0);
	ATA_CMD(device, sata_num, IDENTIFY);
	if(IDE_INB(device, sata_num, ATA_STATUS_REG) == 0)
		return;

	ide_poll(device, sata_num);

	uint16_t buffer[256];
	for(uint16_t i=0; i<256; i++)
		buffer[i] = IDE_INW(device, sata_num, ATA_DATA_REG);
}

static void lba_select(ide_device_t* device, uint8_t sata_num, unsigned int lba)
{
	IDE_OUTB(device, sata_num, ATA_LBA0_REG, lba);
	IDE_OUTB(device, sata_num, ATA_LBA1_REG, lba >> 8);
	IDE_OUTB(device, sata_num, ATA_LBA2_REG, lba >> 16);
	IDE_OUTB(device, sata_num, ATA_FLAGS_REG, (sata_num&1 ? MASTER : SLAVE) | ((lba>>24)&0x0F));
}

void* atapio_read_sect(ide_device_t* device, uint8_t sata_num, unsigned int lba, uint8_t sector_count, void* buffer)
{
	IDE_OUTB(device, sata_num, ATA_SECCOUNT0_REG, sector_count);

	lba_select(device, sata_num, lba);
	ATA_CMD(device, sata_num, READ_PIO);

	uint16_t* ptr = (uint16_t*)buffer;
	for(uint8_t i=0; i<sector_count; i++)
	{
		ide_poll(device, sata_num);
		for(uint16_t j=0; j<256; j++)
			*(ptr++) = IDE_INW(device, sata_num, ATA_DATA_REG);
	}

	return buffer;
}

void atapio_write_sect(ide_device_t* device, uint8_t sata_num, unsigned int lba, uint8_t sector_count, void* buffer)
{
	IDE_OUTB(device, sata_num, ATA_SECCOUNT0_REG, sector_count);
	lba_select(device, (sata_num-1)/2, lba);
	ATA_CMD(device, sata_num, WRITE_PIO);

	uint16_t* ptr = (uint16_t*)buffer;
	for(uint8_t i=0; i<sector_count; i++)
	{
		ide_poll(device, sata_num);
		for(uint16_t j=0; j<256; j++)
			IDE_OUTW(device, sata_num, ATA_DATA_REG, *(ptr++));
	}
}