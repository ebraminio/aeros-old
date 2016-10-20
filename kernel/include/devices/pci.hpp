#ifndef __DEVICES_PCI_HPP__
#define __DEVICES_PCI_HPP__

#include <cstdint>

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

#define PCI_CLASS_NUM 18

class PCI_device
{
	public:
		uint8_t _bus, _dev, _func;
		uint32_t _pci_base;
		uint16_t _dev_id, _ven_id;
		uint8_t _classcode, _subclass, _prog_if, _rev_id;
		uint16_t _subsyst_id, _subsyst_ven_id;
		uint8_t _header_type;
		void* _mem_base;
		uint32_t _iobase;
		uint8_t _irq_num;

		PCI_device(uint8_t bus=0, uint8_t dev = 0, uint8_t func=0);
		uint32_t read(uint32_t reg) const;
		void write(uint32_t reg, uint32_t value) const;
		uint32_t get_ids(void);
		uint32_t get_interface(void);
		void set_status(uint16_t status) const;
};

void pci_init(void);

#endif