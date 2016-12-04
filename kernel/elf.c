#include <elf.h>
#include "elf.h"
#include <stdlib.h>
#include <string.h>
#include "io/log.h"
#include "mem/vmem.h"
#include <sys/mman.h>

#define ELF_SECTIONS(hdr) ((Elf32_Shdr*)((char*)hdr+hdr->e_shoff))
#define ELF_SECTION(hdr, n) ((void*)((char*)hdr + ELF_SECTIONS(hdr)[n].sh_offset))

void elf_loadprog(Elf32_Ehdr* header)
{
	Elf32_Phdr* phdr = (Elf32_Phdr*) ((char*)header+header->e_phoff);
	for(uint8_t i=0; i<header->e_phnum; i++)
		if(phdr[i].p_type == PT_LOAD)
		{
			uintptr_t ptr = (uintptr_t)malloc(phdr[i].p_memsz);
			vmap(phdr[i].p_vaddr, ptr, phdr[i].p_memsz, true);
			memcpy((void*)phdr[i].p_vaddr, (char*)header + phdr[i].p_offset, phdr[i].p_filesz);
			mprotect((void*)phdr[i].p_vaddr, phdr[i].p_memsz, phdr[i].p_flags & PF_W ? PROT_WRITE : PROT_NONE);
		}
}

void elf_loadsects(Elf32_Ehdr* header)
{
	Elf32_Shdr* sections = ELF_SECTIONS(header);
	char* names = (char*) ELF_SECTION(header, header->e_shstrndx);
	for(uint8_t i=0; i<header->e_shnum; i++)
		if(sections[i].sh_flags & SHF_ALLOC)
		{
			if(header->e_type == ET_REL)
			{
				sections[i].sh_addr = (Elf32_Addr)malloc(sections[i].sh_size);
				if(strcmp(names+sections[i].sh_name, ".text") == 0)
					header->e_entry += sections[i].sh_addr;
			}
			memcpy((void*)sections[i].sh_addr, ELF_SECTION(header, i), sections[i].sh_size);
		}
}

static int elf_rel(Elf32_Ehdr* header)
{
	Elf32_Shdr* sections = ELF_SECTIONS(header);

	for(uint8_t i=0; i<header->e_shnum; i++)
		if(sections[i].sh_type == SHT_REL)
		{
			Elf32_Rel* reltab = (Elf32_Rel*) ELF_SECTION(header, i);
			Elf32_Shdr* rel_sec = &sections[sections[i].sh_info];
			if(!(rel_sec->sh_flags & SHF_ALLOC))
				continue;
			if(sections[i].sh_link == SHN_UNDEF)
				panic("Relocation error: value undefined");

			Elf32_Shdr* symtab_sec = &sections[sections[i].sh_link];
			char* names = (char*) ELF_SECTION(header, symtab_sec->sh_link);
			Elf32_Sym* symtab = (Elf32_Sym*) ELF_SECTION(header, sections[i].sh_link);

			for(uint32_t j=0; j<sections[i].sh_size/sections[i].sh_entsize; j++)
			{
				if(ELF32_R_SYM(reltab[j].r_info) == SHN_UNDEF)
					panic("Relocation error: value undefined");
				if(ELF32_R_SYM(reltab[j].r_info) >= symtab_sec->sh_size / symtab_sec->sh_entsize)
					panic("Relocation error: symbol index out of range");

				Elf32_Sym* symbol = &symtab[ELF32_R_SYM(reltab[j].r_info)];
				if(ELF32_ST_BIND(symbol->st_info) == STB_WEAK)
					continue;	// Skip weak references
				if(symbol->st_shndx == SHN_UNDEF)
					panic("Relocation error: external symbol %s", names+symbol->st_name);

				uintptr_t value;
				if(symbol->st_shndx == SHN_ABS)
					value = symbol->st_value;
				else value = sections[symbol->st_shndx].sh_addr + symbol->st_value;
				if(*(names+symbol->st_name))
					nopanic("Relocated %s\n", names+symbol->st_name);

				uintptr_t* ref = (uintptr_t*)(rel_sec->sh_addr + reltab[j].r_offset);
				switch(ELF32_R_TYPE(reltab[j].r_info))
				{
					case R_386_NONE: break;
					case R_386_32: *ref += value; break;
					case R_386_PC32: *ref += value-(uintptr_t)ref; break;
					default: panic("Unsupported relocation type");
				}
			}
		}

	return 0;
}

void* elf_load(void* buf)
{
	Elf32_Ehdr* header = (Elf32_Ehdr*) buf;
	//Ignore version and OS ABI
	const uint64_t expexted_header = ELFMAG0
		|ELFMAG1 << 0x08
		|ELFMAG2 << 0x10
		|ELFMAG3 << 0x18
		|(uint64_t)ELFCLASS32 << 0x20
		|(uint64_t)ELFDATA2LSB << 0x28;

	if((*(uint64_t*)header & 0xFFFFFFFFFFFF) != expexted_header)
	{
		panic("Not an I386 32bit LSB ELF file");
		return NULL;
	}

	switch(header->e_type)
	{
		case ET_EXEC:

		case ET_REL:
			if(header->e_phnum)
				elf_loadprog(header);
			else elf_loadsects(header);
			if(header->e_type != ET_REL || elf_rel(header) == 0)
				return (void*)header->e_entry;
			return NULL;
		default:
			panic("Unsupported object type");
			return NULL;
	}
}