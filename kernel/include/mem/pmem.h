#ifndef __MEM_PMEM_H__
#define __MEM_PMEM_H__

#include <stdint.h>
#include "multiboot.h"

void pmem_init(multiboot_info_t* multiboot_info);
void* palloc(void);
void pfree(void* block);

#endif