#ifndef __DEVICES_PCI_H__
#define __DEVICES_PCI_H__

#include <stdint.h>

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
} pci_device_t;

void pci_init(void);

#endif