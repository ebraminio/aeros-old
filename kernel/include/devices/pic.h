#ifndef __DEVICES_PIC_H__
#define __DEVICES_PIC_H__

#include "cpu/regs.h"

void irq_install_handler(uint32_t irq, void (*handler)(regs_t*));
void pic_remap(void);
void mask_irq(uint8_t irq);
void unmask_irq(uint8_t irq);

#endif