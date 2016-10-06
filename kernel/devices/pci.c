#include "devices/pci.h"

#include <sys/io.h>
#include <string.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC
#define PCI_HDR_MULTI_FUNC 0x80

#define PCI_ADDR(pci_device, reg) (uint32_t)(((uint32_t)pci_device->bus << 16) \
	| ((uint32_t)pci_device->device << 11) \
	| ((uint32_t)pci_device->func << 8) \
	| (reg & 0xfc) \
	| 0x80000000)

#define MASS_STORAGE_CONTROLLER		0x01
#define IDE_CONTROLLER				0x01
#define FLOPPY_DISK_CONTROLLER		0x02
#define ATA_CONTROLLER				0x05
#define SATA_CONTROLLER				0x06

#define NETWORK_CONTROLLER			0x02
#define ETHERNET_CONTROLLER			0x00
#define OTHER_NETWORK_CONTROLLER	0x80

#define DISPLAY_CONTROLLER			0x03
#define VGA_CONTROLLER				0x0000

#define MULTIMEDIA_CONTROLLER		0x04
#define VIDEO_DEVICE				0x00
#define AUDIO_DEVICE				0x01

#define BRIDGE_DEVICE				0x06
#define HOST_BRIDGE					0x00
#define ISA_BRIDGE					0x01
#define PCI_PCI_BRIDGE				0x04

#define SERIAL_BUS_CONTROLLER		0x0C
#define USB_CONTROLLER				0x03

#define WIRELESS_CONTROLLER			0x0D

static uint32_t pci_read(pci_device_t* device, uint8_t reg)
{
	outl(CONFIG_ADDRESS, PCI_ADDR(device, reg));
	return inl(CONFIG_DATA);
}

static void pci_write(pci_device_t* device, uint8_t reg, uint32_t value)
{
	outl(CONFIG_ADDRESS, PCI_ADDR(device, reg));
	outl(CONFIG_DATA, value);
}

uint32_t get_ids(pci_device_t* device)
{
	uint32_t ids = pci_read(device, 0);
	device->dev_id = (uint16_t)(ids>>16);
	device->ven_id = (uint16_t)ids&0xFFFF;
	return ids;
}

uint32_t get_interface(pci_device_t* device)
{
	uint32_t interface = pci_read(device, 8);
	device->classcode = interface>>24;
	device->subclass = (interface>>16)&0xFF;
	device->prog_if = (interface>>8)&0xFF;
	device->rev_id = interface&0xFF;
	return interface;
}

uint8_t get_header_type(pci_device_t* device)
{
	return device->header_type = (uint8_t)(pci_read(device, 0xC)>>16)&0xFF;
}

pci_device_t storage_devices[5] = {0};
pci_device_t network_devices[5] = {0};
pci_device_t display_devices[5] = {0};
pci_device_t serial_bus_devices[5] = {0};

pci_device_t* storage_device_slot = &storage_devices[0];
pci_device_t* network_device_slot = &network_devices[0];
pci_device_t* display_device_slot = &display_devices[0];
pci_device_t* serial_bus_device_slot = &serial_bus_devices[0];

static void check_func(pci_device_t* device)
{
	get_interface(device);

	switch(device->classcode)
	{
		case MASS_STORAGE_CONTROLLER:
			switch(device->subclass)
			{
				case IDE_CONTROLLER:
					if(storage_device_slot)
						memcpy(storage_device_slot++, device, sizeof(*device));
					break;
				case ATA_CONTROLLER:
					if(storage_device_slot)
						memcpy(storage_device_slot++, device, sizeof(*device));
					break;
				case SATA_CONTROLLER:
					if(storage_device_slot)
						memcpy(storage_device_slot++, device, sizeof(*device));
					break;
			}
			if(storage_device_slot > &storage_devices[sizeof(storage_devices)-1])
				storage_device_slot = NULL;
			break;
		case NETWORK_CONTROLLER:
			switch(device->subclass)
			{
				case ETHERNET_CONTROLLER:
					if(network_device_slot)
						memcpy(network_device_slot++, device, sizeof(*device));
					break;
			}
			if(network_device_slot > &network_devices[sizeof(network_devices)-1])
				network_device_slot = NULL;
			break;
		case DISPLAY_CONTROLLER:
			switch(device->subclass)
			{
				default: break;
			}
			break;
		case MULTIMEDIA_CONTROLLER:
			switch(device->subclass)
			{
				default: break;
			}
			break;
		case BRIDGE_DEVICE:
			switch(device->subclass)
			{
				default: break;
			}
			break;
		case SERIAL_BUS_CONTROLLER:
			switch(device->subclass)
			{
				case USB_CONTROLLER:
					if(serial_bus_device_slot)
						memcpy(serial_bus_device_slot++, device, sizeof(*device));
					break;
			}
			if(serial_bus_device_slot > &serial_bus_devices[sizeof(serial_bus_devices)-1])
				serial_bus_device_slot = NULL;
			break;
		case WIRELESS_CONTROLLER:
			switch(device->subclass)
			{
				default:
					if(network_device_slot)
						memcpy(network_device_slot++, device, sizeof(*device));
			}
			if(network_device_slot > &network_devices[sizeof(network_devices)-1])
				network_device_slot = NULL;
			break;
	}
}

static void check_device(uint8_t bus, uint8_t device)
{
	pci_device_t pci_device = {.bus=bus, .device=device, .func=0};
	get_ids(&pci_device);

	if(pci_device.ven_id == 0xFFFF)
		return;

	check_func(&pci_device);

	if(get_header_type(&pci_device) & PCI_HDR_MULTI_FUNC)
		for(pci_device.func=1; pci_device.func<8; pci_device.func++)
		{
			get_ids(&pci_device);
			if(pci_device.ven_id != 0xFFFF)
				check_func(&pci_device);
		}
}

static void check_bus(uint8_t bus)
{
	for(uint8_t device=0; device<32; device++)
		check_device(bus, device);
}

void pci_init(void)
{
	pci_device_t device = {.bus=0, .device=0, .func=0};

	if(get_header_type(&device) & PCI_HDR_MULTI_FUNC)
		for(; device.func < 8; device.func++)
		{
			get_ids(&device);
			if(device.dev_id != 0xFFFF)
				break;
			check_bus(device.func);
		}
	else check_bus(0);
}