#define ASM_FILE

#define DEF_ISR(NUM) .global _isr##NUM## ;\
.type _isr##NUM##, @function ;\
_isr##NUM##: \
	xchgw %bx, %bx ;\
	cli ;\
	push $0 ;\
	push $##NUM## ;\
	jmp _isr_common_stub

#define DEF_ISR_ERR(NUM) .global _isr##NUM## ;\
.type _isr##NUM##, @function ;\
_isr##NUM##: \
	xchgw %bx, %bx ;\
	cli ;\
	push $##NUM## ;\
	jmp _isr_common_stub

.section .text

DEF_ISR(0)
DEF_ISR(1)
DEF_ISR(2)
DEF_ISR(3)
DEF_ISR(4)
DEF_ISR(5)
DEF_ISR(6)
DEF_ISR(7)
DEF_ISR_ERR(8)
DEF_ISR(9)
DEF_ISR_ERR(10)
DEF_ISR_ERR(11)
DEF_ISR_ERR(12)
DEF_ISR_ERR(13)
DEF_ISR_ERR(14)
DEF_ISR(15)
DEF_ISR(16)
DEF_ISR(17)
DEF_ISR(18)
DEF_ISR(19)
DEF_ISR(20)
DEF_ISR(21)
DEF_ISR(22)
DEF_ISR(23)
DEF_ISR(24)
DEF_ISR(25)
DEF_ISR(26)
DEF_ISR(27)
DEF_ISR(28)
DEF_ISR(29)
DEF_ISR(30)
DEF_ISR(31)

.global _isr_common_stub
.type _isr_common_stub, @function
_isr_common_stub:
	pusha
	mov %cr3, %eax
	push %eax
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax	# Load the Kernel Data Segment descriptor
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %esp, %eax
	push %eax
	#mov fault_handler, %eax
	#call *%eax	# A special call, preserves the 'eip' register
	call fault_handler
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add $8, %esp	# Cleans up the pushed error code and pushed ISR number
	iret

#define DEF_IRQ(NUM) .global _irq##NUM## ;\
.type _irq##NUM##, @function ;\
_irq##NUM##: ;\
	cli ;\
	push $0 ;\
	push $(##NUM##+32) ;\
	jmp irq_common_stub

DEF_IRQ(0)
DEF_IRQ(1)
DEF_IRQ(2)
DEF_IRQ(3)
DEF_IRQ(4)
DEF_IRQ(5)
DEF_IRQ(6)
DEF_IRQ(7)
DEF_IRQ(8)
DEF_IRQ(9)
DEF_IRQ(10)
DEF_IRQ(11)
DEF_IRQ(12)
DEF_IRQ(13)
DEF_IRQ(14)
DEF_IRQ(15)
DEF_IRQ(128)

.type irq_common_stub, @function
irq_common_stub:
	pusha
	mov %cr3, %eax
	push %eax
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %esp, %eax
	push %eax
	call irq_handler
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	pop %eax	#Drop CR3
	popa
	add $8, %esp
	iret

.global _set_idtr
.type _set_idtr, @function
_set_idtr:
	xchgw %bx, %bx
	mov 4(%esp), %eax
	lidt (%eax)
	sti
	ret
