.section .text

.global _set_gdtr
.type _set_gdtr, @function
_set_gdtr:
	xchgw %bx, %bx
	mov 4(%esp), %eax
	lgdt (%eax)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ljmp $0x8, $me
me:	ret
.size _set_gdtr, . - _set_gdtr
