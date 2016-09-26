#include "cpu/acpi.h"
#include "io/log.h"
#include <string.h>
#include "cpu/acpi_tables.h"
#include <sys/io.h>

static uint8_t acpi_check(const acpi_hdr_t* hdr, uint32_t sig)
{
	if(hdr->sig != sig)
		return 1;
	uint8_t sum = 0;
	for(uint32_t i=0; i<hdr->length; i++)
		sum += ((uint8_t*)hdr)[i];
	return sum;
}

uint16_t SLP_TYPa, SLP_TYPb;
uint32_t smi_cmd_port = 0;
uint8_t acpi_enable_cmd, acpi_disable_cmd;
uint32_t pm1a_ctrl_blk, pm1b_ctrl_blk;
uint8_t shutdown_available = 0;

static void find_acpi_s5(dsdt_t* sdt)
{
	const uint32_t dsdt_aml_len = sdt->acpi_hdr.length - sizeof(acpi_hdr_t);
	for(uint32_t k=0; k<dsdt_aml_len; k++)
		if(*(uint32_t*)&sdt->aml[k] == S5_SIG)
		{
			// TODO: Check for valid AML structure
			k += 5;
			k += ((sdt->aml[k] & 0xC0)>>6) + 2;	// PkgLength size
			if(sdt->aml[k] == 0x0A) k++;	// Skip byteprefix
			SLP_TYPa = sdt->aml[k]<<10;
			k++;
			if(sdt->aml[k] == 0x0A) k++;
			SLP_TYPb = sdt->aml[k]<<10;
			shutdown_available = 1;
			return;
		}
}

void acpi_init(void)
{
	// Find RDSP
	// It is located in the 1st KB of EBDA or in BIOS area (0xE0000 - 0xFFFFF),
	// on a 16 byte boundary
	// Search only in BIOS area
	for(uint8_t* i=(uint8_t*)0xE0000; i<=(uint8_t*)0xFFFFF; i+=16/sizeof(i))
		if(*(uint64_t*)i == RSDP_SIG)
		{
			uint8_t sum = 0;
			for(uint8_t j=0; j<sizeof(rsdp_t); j++)
				sum += i[j];
			if(sum != 0)
				panic("RSDP is corrupted");

			const rsdp_t* rsdp = (rsdp_t*)i;
			if(rsdp->revision == 2)
			{
				sum = 0;
				for(uint8_t j=sizeof(rsdp_t); j<sizeof(rsdp_ext_t); j++)
					sum += i[j];
				if(sum != 0)
					panic("Extended RSDP is corrupted");
			}

			const rsdt_t* rsdt = (rsdt_t*)rsdp->rsdt;
			if(acpi_check(&rsdt->acpi_hdr, RSDT_SIG) != 0)
				panic("Pointed RSDT is invalid or corrupted");

			const uint8_t sdt_num = (rsdt->acpi_hdr.length - sizeof(acpi_hdr_t))/4;
			for(uint8_t j=0; j<sdt_num; j++)
				switch(((acpi_hdr_t*)rsdt->sdts[j])->sig)
				{
					case FADT_SIG:;
						const fadt_t* fadt = (fadt_t*)(acpi_hdr_t*)rsdt->sdts[j];
						smi_cmd_port = fadt->smi_cmd_port;
						acpi_enable_cmd = fadt->acpi_enable;
						acpi_disable_cmd = fadt->acpi_disable;
						pm1a_ctrl_blk = fadt->pm1a_ctrl_blk;
						pm1b_ctrl_blk = fadt->pm1b_ctrl_blk;

						dsdt_t* dsdt = (dsdt_t*)fadt->dsdt;
						if(acpi_check(&dsdt->acpi_hdr, DSDT_SIG) != 0)
							panic("Pointed DSDT is invalid or corrupted");
						find_acpi_s5(dsdt);
						break;
					case SSDT_SIG: find_acpi_s5((dsdt_t*)rsdt->sdts[j]);
						break;
					case MADT_SIG:;
						const madt_t* madt = (madt_t*)(acpi_hdr_t*)rsdt->sdts[j];
						break;
				}
			if(!shutdown_available)
				panic("_S5 not found");
			return;
		}
}

// Enabling ACPI disable many automated things
// but allow us to control the system power
void acpi_enable(void)
{
	if((inw(pm1a_ctrl_blk) & SCI_EN) == 0)	// ACPI not enable, let's try dot it
		if(smi_cmd_port)	// SMI command port has been previously found
		{
			outb(smi_cmd_port, acpi_enable_cmd);
			uint16_t i = 0;	// Wait 3s for ACPI enable

			for(; i<300; i++)
				if(inw(pm1a_ctrl_blk) & SCI_EN)
					break;
//				else sleep(10);
			// TODO implement sleep, at least with the PIT
			if(pm1b_ctrl_blk)
				for(; i<300; i++)
					if(inw(pm1b_ctrl_blk) & SCI_EN)
						break;
//					else sleep(10);

			if(i==300)
				panic("Failed to enable ACPI");
		}
}

__attribute__((noreturn))
void acpi_poweroff(void)
{
	if(!shutdown_available)
		panic("ACPI shutdown not available");
	acpi_enable();
	outw(pm1a_ctrl_blk, SLP_TYPa | SLP_EN);
	if(pm1b_ctrl_blk)
		outw(pm1b_ctrl_blk, SLP_TYPa | SLP_EN);

	panic("ACPI shutdown failed");
}