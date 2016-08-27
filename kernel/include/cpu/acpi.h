#ifndef __CPU_ACPI_H__
#define __CPU_ACPI_H__

void acpi_init(void);
void acpi_poweroff(void) __attribute__ ((noreturn));

#endif