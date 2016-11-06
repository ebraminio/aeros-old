#ifndef __DEVICES_PCI_H__
#define __DEVICES_PCI_H__

#include <stdint.h>

#define DEVICE_ID		0x0010FFFF
#define VENDOR_ID		0x0000FFFF
#define STATUS			0x0410FFFF
#define COMMAND			0x0400FFFF
#define CLASS_CODE		0x081800FF
#define SUBCLASS		0x081000FF
#define PROG_IF			0x080800FF
#define REVISION_ID		0x080000FF
#define BIST			0x0C1800FF
#define HEADER_TYPE		0x0C1000FF
#define LATENCY_TIMER	0x0C0800FF
#define CACHE_LINE_SIZE	0x0C0000FF
#define BAR0			0x10000000
#define BAR1			0x14000000
#define SECONDARY_BUS	0x180800FF
#define INTERRUPT_LINE	0x3C0000FF

typedef struct
{
	uint8_t bus, device, func;
	uint16_t dev_id, ven_id;
	uint8_t classcode, subclass, prog_if, rev_id;
	uint16_t subsyst_id, subsyst_ven_id;
	uint8_t header_type;
	void* mem_base;
	uint32_t iobase;
	uint8_t irq_num;
	uint32_t bar[6];
} pci_device_t;

#define PCI_CLASS_NUM 18
const char* const pci_classNames[PCI_CLASS_NUM];

extern pci_device_t* storage_devices[5];
extern pci_device_t* network_devices[5];
extern pci_device_t* display_devices[5];
extern pci_device_t* serial_bus_devices[5];

void pci_init(void);
uint32_t pci_read(pci_device_t* device, uint32_t reg);
void pci_write(pci_device_t* device, uint8_t reg, uint32_t value);
uint32_t get_bar(pci_device_t* device, uint8_t bar_num);

#endif