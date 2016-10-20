#include "devices/pci.hpp"

#include <sys/io.h>
#include <string.h>
#include "io/log.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC
#define PCI_HDR_MULTI_FUNC 0x80

#define PCI_ADDR(pci_device, reg) (uint32_t)(((uint32_t)pci_device->bus << 16) \
	| ((uint32_t)pci_device->device << 11) \
	| ((uint32_t)pci_device->func << 8) \
	| (reg & 0xfc) \
	| 0x80000000)
#define PCI_REG(X) (X&0xFF000000)

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

const char* const pci_classNames[PCI_CLASS_NUM] =
{
	"OLD_DEVICE",
	"Mass Storage Controller",
	"Network Controller",
	"Display Controller",
	"Multimedia Controller",
	"Memory Controller",
	"Bridge Device",
	"Simple Communication Controllers",
	"Base System Peripheral",
	"Input Device",
	"Docking Station",
	"Processors",
	"Serial Bus Controllers",
	"Wireless Controllers",
	"Intelligent I/O Controllers",
	"Satellite Communication Controllers",
	"Encryption/Decryption Controllers",
	"Data Acquisition and Signal Processing Controllers",
};

PCI_device storage_devices[5] = {0};
PCI_device network_devices[5] = {0};
PCI_device display_devices[5] = {0};
PCI_device serial_bus_devices[5] = {0};

static PCI_device* storage_device_slot = &storage_devices[0];
static PCI_device* network_device_slot = &network_devices[0];
static PCI_device* display_device_slot = &display_devices[0];
static PCI_device* serial_bus_device_slot = &serial_bus_devices[0];

/*static uint32_t get_bar(pci_device_t* device, uint8_t bar_num)
{
    device->header_type = pci_read(device, HEADER_TYPE);

    if(bar_num>6 || device->header_type>1 || (device->header_type==1 && bar_num>1))
        panic("Invalid BAR %d on PCI %d.%d.%d",
                bar_num, device->bus, device->device, device->func);
    return pci_read(device, BAR0+(4*bar_num)<<0x18);
}*/
static void check_bus(uint8_t bus);

static void check_func(PCI_device* device)
{
	device->get_interface();

	switch(device->_classcode)
	{
		case MASS_STORAGE_CONTROLLER:
			switch(device->_subclass)
			{
				case IDE_CONTROLLER:
					if(storage_device_slot)
						*(storage_device_slot++) = PCI_device(*device);
					break;
				case ATA_CONTROLLER:
					if(storage_device_slot)
						*(storage_device_slot++) = PCI_device(*device);
					break;
				case SATA_CONTROLLER:
					if(storage_device_slot)
						*(storage_device_slot++) = PCI_device(*device);
					break;
			}
			if(storage_device_slot > &storage_devices[sizeof(storage_devices)-1])
				storage_device_slot = NULL;
			break;
		case NETWORK_CONTROLLER:
			switch(device->_subclass)
			{
				case ETHERNET_CONTROLLER:
					if(network_device_slot)
						*(network_device_slot++) = PCI_device(*device);
					break;
			}
			if(network_device_slot > &network_devices[sizeof(network_devices)-1])
				network_device_slot = NULL;
			break;
		case DISPLAY_CONTROLLER:
			switch(device->_subclass)
			{
				case VGA_CONTROLLER:
					if(display_device_slot)
						*(display_device_slot++) = PCI_device(*device);
					break;
			}
			if(display_device_slot > &display_devices[sizeof(display_devices)-1])
				display_device_slot=0;
			break;
		case MULTIMEDIA_CONTROLLER:
			switch(device->_subclass)
			{
				case VIDEO_DEVICE:
					break;
				case AUDIO_DEVICE:
					break;
			}
			break;
		case BRIDGE_DEVICE:
			switch(device->_subclass)
			{
				case HOST_BRIDGE:
					break;
				case PCI_PCI_BRIDGE:
					check_bus(device->read(SECONDARY_BUS));
					break;
			}
			break;
		case SERIAL_BUS_CONTROLLER:
			switch(device->_subclass)
			{
				case USB_CONTROLLER:
					if(serial_bus_device_slot)
						*(serial_bus_device_slot++) = PCI_device(*device);
					break;
			}
			if(serial_bus_device_slot > &serial_bus_devices[sizeof(serial_bus_devices)-1])
				serial_bus_device_slot = NULL;
			break;
		case WIRELESS_CONTROLLER:
			switch(device->_subclass)
			{
				default:
					if(network_device_slot)
						*(network_device_slot++) = PCI_device(*device);
			}
			if(network_device_slot > &network_devices[sizeof(network_devices)-1])
				network_device_slot = NULL;
			break;
	}
}

static void check_dev(uint8_t bus, uint8_t dev)
{
	PCI_device device(bus, dev, 0);

	if((device.get_ids() & 0xFFFF) == 0xFFFF)
		return;

	check_func(&device);

	if(device.read(HEADER_TYPE) & PCI_HDR_MULTI_FUNC)
		for(device._func=1; device._func<8; device._func++)
		{
			device.get_ids();
			if(device._ven_id != 0xFFFF)
				check_func(&device);
		}
}

static void check_bus(uint8_t bus)
{
	for(uint8_t dev=0; dev<32; dev++)
		check_dev(bus, dev);
}

void pci_init(void)
{
	PCI_device device;

	if(device.read(HEADER_TYPE) & PCI_HDR_MULTI_FUNC)
		for(; device._func < 8; device._func++)
		{
			if((device.get_ids()>>16) != 0xFFFF)
				break;
			check_bus(device._func);
		}
	else check_bus(0);
}