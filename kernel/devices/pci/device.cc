#include "devices/pci.hpp"
#include <sys/io.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC
#define PCI_HDR_MULTI_FUNC 0x80

#define PCI_ADDR(reg) (this->_pci_base | ((reg) & 0xfc))
#define PCI_REG(X) ((X)&0xFF000000)

PCI_device::PCI_device(uint8_t bus, uint8_t dev, uint8_t func) : _bus(bus),
	_dev(dev), _func(func),
	_pci_base(0x80000000 | ((uint32_t)bus<<16) | ((uint32_t)dev<<11) | \
		((uint16_t)func<<8))
{}

uint32_t PCI_device::read(uint32_t reg) const
{
	outl(CONFIG_ADDRESS, PCI_ADDR(reg>>24));
	uint32_t data = inl(CONFIG_DATA);
	data >>= (reg>>16)&0xFF;
	if(reg&0xFFFF)
		data &= reg&0xFFFF;
	return data;
}

void PCI_device::write(uint32_t reg, uint32_t value) const
{
	outl(CONFIG_ADDRESS, PCI_ADDR(reg));
	outl(CONFIG_DATA, value);
}

uint32_t PCI_device::get_ids(void)
{
	uint32_t ids = this->read(PCI_REG(DEVICE_ID));
	_dev_id = (uint16_t)(ids>>16);
	_ven_id = (uint16_t)ids&0xFFFF;
	return ids;
}

uint32_t PCI_device::get_interface(void)
{
	uint32_t interface = this->read(PCI_REG(CLASS_CODE));
	_classcode = interface>>0x18;
	_subclass = (interface>>0x10)&0xFF;
	_prog_if = (interface>>8)&0xFF;
	_rev_id = interface&0xFF;
	return interface;
}

void PCI_device::set_status(uint16_t status) const
{
	uint32_t reg = this->read(PCI_REG(STATUS))&0xFFFF;
	this->write(PCI_REG(STATUS)>>24, reg|(status<<16));
}