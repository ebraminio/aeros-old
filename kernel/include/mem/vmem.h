#ifndef __MEM_VMEM_H__
#define __MEM_VMEM_H__

#include <stdint.h>

void vmem_init(void);
void vmap(uintptr_t vstart, uintptr_t pstart, size_t size);

#endif