#ifndef __MEM_VMEM_H__
#define __MEM_VMEM_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define IDENTITY_MAP(p, s, w) vmap(p, p, s, w)

void vmem_init(void);
void vmap(uintptr_t vstart, uintptr_t pstart, size_t size, bool writeable);

#endif