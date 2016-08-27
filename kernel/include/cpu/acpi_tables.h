#ifndef __CPU_ACPI_TABLES_H__
#define __CPU_ACPI_TABLES_H__

#define RSDP_SIG 0x2052545020445352	// "RSD PTR "
#define S5_SIG 0x5F35535F	// "_S5_"

#define SCI_EN 1
#define SLP_EN (1<<13)

typedef struct
{
	uint64_t sig;
	uint8_t checksum;
	uint8_t oem_id[6];
	uint8_t revision;
	uint32_t rsdt;
} rsdp_t;

typedef struct
{
	rsdp_t legacy_desc;
	uint32_t length;
	uint64_t xsdt;
	uint8_t ext_checksum;
	uint8_t reserved[3];	// TODO: Find if they are included in checksum
} rsdp_ext_t;

typedef struct
{
	uint32_t sig;
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	uint8_t oem_id[6];
	uint8_t oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
} acpi_hdr_t;

#define RSDT_SIG 0x54445352
typedef struct
{
	acpi_hdr_t acpi_hdr;
	uint32_t sdts[];
} rsdt_t;

#define DSDT_SIG 0x54445344
typedef struct
{
	acpi_hdr_t acpi_hdr;
	uint8_t aml[];
} dsdt_t;

#define SSDT_SIG 0x54445353
typedef dsdt_t ssdt_t;

#define FADT_SIG 0x50434146	// "FACP"
typedef struct
{
	uint8_t address_space;
	uint8_t bitwidth;
	uint8_t bitoffset;
	uint8_t acess_size;	// In bytes
	uint64_t address;
} gas_t;

typedef struct
{
	acpi_hdr_t acpi_hdr;

	uint32_t firmware_ctrl;
	uint32_t dsdt;
	uint8_t reserved1;
	uint8_t preferred_pwr_mgmt_profile;	// TODO: document possible values
	uint16_t sci_interrupt;				// IRQ used by ACPI hardware, including power button
	uint32_t smi_cmd_port;
	uint8_t acpi_enable;
	uint8_t acpi_disable;
	uint8_t s4bios_req;
	uint8_t pstate_control;
	uint32_t pm1a_event_blk;
	uint32_t pm1b_event_blk;
	uint32_t pm1a_ctrl_blk;
	uint32_t pm1b_ctrl_blk;
	uint32_t pm2_ctrl_blk;
	uint32_t pm_timer_blk;
	uint32_t gpe0_blk;
	uint32_t gpe1_blk;
	uint8_t pm1_event_len;
	uint8_t pm1_ctr_len;
	uint8_t pm2_ctrl_len;
	uint8_t todo[18];
	uint16_t boot_arch_flags;
	uint8_t reserved2;
	uint32_t flags;
	gas_t reset_reg;
	uint8_t reset_value;
	uint8_t reserved3[3];
	uint64_t x_firmware_ctrl;
	uint64_t x_dst;
	gas_t todo2[8];
} fadt_t;

typedef struct
{
	uint8_t type;
	uint8_t length;
} madt_entry_t;

#define MADT_SIG 0x43495041
typedef struct
{
	uint32_t local_controller_addr;
	uint32_t flags;
	madt_entry_t int_device_list[];
} madt_t;

#define PROC_LOC_APIC 0
typedef struct
{
	uint8_t apic_proc_id;
	uint8_t apic_id;
	uint32_t flags;
} proc_local_apic_t;

#define IO_APIC 1
typedef struct
{
	uint8_t io_apic_id;
	uint8_t zero;
	uint32_t address;
	uint32_t global_system_interrupt_base;
} io_apic_t;

typedef struct
{
	acpi_hdr_t acpi_hdr;
	uint8_t hw_revid;
	uint8_t comparator_count : 5;
	uint8_t counter_size : 1;
	uint8_t reserved : 1;
	uint8_t legacy_emplacement : 1;
	uint16_t pci_vid;
	gas_t address;
	uint8_t hpet_num;
	uint16_t minimum_tick;
	uint8_t page_protection;
} hpet_t;

#endif