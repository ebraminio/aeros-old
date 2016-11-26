.section .text

.global _dma_completed
.type _dma_completed, @function
_dma_completed:
	push %ebp
	mov %esp, %ebp

	mov $0x0C, %dl
	mov $0xFF, %al
	outb %al, %dx
	push %ebx
	mov $dma_count_ports, %ebx
	push %ecx
	mov 8(%ebp),%ecx
	movzb (%ebx, %ecx, 2), %dx
	pop %ebx
	pop %ecx
redo:
	inb %dx, %al
	mov %al, %bl
	inb %dx, %al
	mov %al, %bh
	inb %dx, %al
	mov %al, %ah
	inb %dx, %al
	xchg %al, %ah
	sub %ax, %bx
	cmp $0xFFC0, %bx
	jl redo
	leave
	ret
.size _dma_completed, . - _dma_completed
