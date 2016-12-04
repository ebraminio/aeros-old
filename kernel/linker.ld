SECTIONS
{
	. = 1M;
	_kernel_base = .;

	.text ALIGN(4K):
	{
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) :
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) :
	{
		_data_start = .;
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) :
	{
		*(COMMON)
		*(.bss)
		. = ALIGN(4K);
		*(.bootstrap_stack)
	}

	_end = .;
}
