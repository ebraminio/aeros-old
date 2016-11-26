#include "devices/pci.h"
#include <sys/io.h>
#include <stdlib.h>
#include "devices/pci/ata.h"

#define MASS_STORAGE_CONTROLLER		0x01
#define IDE_CONTROLLER				0x01

uint8_t mbr[512];

void ide_init(ide_device_t* device)
{
	device->pci_device.header_type = pci_read(&device->pci_device, HEADER_TYPE);

	device->pci_device.bar[0] = get_bar(&device->pci_device, 0);
	device->pci_device.bar[1] = get_bar(&device->pci_device, 1);
	device->pci_device.bar[2] = get_bar(&device->pci_device, 2);
	device->pci_device.bar[3] = get_bar(&device->pci_device, 3);
	device->pci_device.bar[4] = get_bar(&device->pci_device, 4);

	device->channels[0].base = (device->pci_device.bar[0] & 0xFFFFFFFC) + 0x1F0 * (!device->pci_device.bar[0]);
	device->channels[0].ctrl = (device->pci_device.bar[1] & 0xFFFFFFFC) + 0x3F6 * (!device->pci_device.bar[1]);
	device->channels[0].bus_master = (device->pci_device.bar[4] & 0xFFFFFFFC);
	device->channels[1].base = (device->pci_device.bar[2] & 0xFFFFFFFC) + 0x170 * (!device->pci_device.bar[2]);
	device->channels[1].ctrl = (device->pci_device.bar[3] & 0xFFFFFFFC) + 0x3F6 * (!device->pci_device.bar[3]);
	device->channels[1].bus_master = (device->pci_device.bar[4] & 0xFFFFFFFC) + 8;

	pci_write(&device->pci_device, INTERRUPT_LINE>>24, 0xFE);
	device->pci_device.irq_num = pci_read(&device->pci_device, INTERRUPT_LINE);
	device->irq_assign_req = device->pci_device.irq_num == 0xFE;
}