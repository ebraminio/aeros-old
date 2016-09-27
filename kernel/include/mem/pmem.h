#ifndef __MEM_PMEM_H__
#define __MEM_PMEM_H__

#include <stdint.h>

void pmem_init(uint32_t mem_lower, uint32_t mem_upper, uint32_t mmap_addr, uint32_t mmap_length);
void* palloc(void);
void pfree(void* block);

#endif