.global divide_error,debug,nmi,int3 ,overflow, bounds, invalid_op
.global double_fault,coprocessor_segment_overrun, invalid_tss, segment_not_present, stack_segment
.global general_protection, coprocessor_error,irq13,reserved

divide_error:
	pushl $do_divide_error
no_error_code:
	xchgl %eax,(%esp)
	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %edi
	pushl %esi
	pushl %ebp
	push %ds
	push %es
	push %fs
	pushl $0
	lea 44(%esp),%edx
	pushl %edx
	movl $0x10, %edx
	mov %dx, %ds
	mov %dx, %es
	mov %dx, %fs
	call *%eax
	addl $8,%esp
	pop %fs
	pop %es
	pop %ds
	popl %ebp
	popl %esi
	popl %edi
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	iret

debug:
	pushl $do_int3
	jmp no_error_code

nmi:
	pushl $do_nmi
	jmp no_error_code


