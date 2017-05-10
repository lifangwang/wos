.equ SIG_CHLD	,17

.equ EAX		, 0x00
.equ EBX		, 0x04
.equ ECX		, 0x08
.equ EDX		, 0x0C
.equ FS		, 0x10
.equ ES		, 0x14
.equ DS		, 0x18
.equ EIP	,0x1C
.equ CS		, 0x20
.equ EFLAGS		, 0x24
.equ OLDESP		, 0x28
.equ OLDSS		, 0x2C

.equ state,0
.equ count, 4
.equ priority , 8
.equ signal , 12
.equ sigaction , 16
.equ blocked , (33*16)
.equ nr_system_calls , 72

.global system_call,sys_fork,timer_interrupt

.align 2
bad_sys_call:
	mov $-1 , %eax
	iret

.align 2
reschedule:
	push $ret_from_sys_call
	jmp schedule

.align 2
system_call:
	cmpl $nr_system_calls-1,%eax
	ja bad_sys_call
	push %ds
	push %es
	push %fs
	pushl %edx
	pushl %ecx
	pushl %ebx
	movl $0x10,%edx
	mov %dx, %ds
	mov %dx, %es
	movl $0x17, %edx
	mov %dx, %fs
	call sys_call_table(,%eax,4)
	pushl %eax
	movl current ,%eax
	cmpl $0, state(%eax)
	jne reschedule
	cmpl $0, count(%eax)
	je reschedule
ret_from_sys_call:
	movl current, %eax
	cmpl task, %eax
	je 3f
	cmpw $0x0f,CS(%esp)
	jne 3f
	cmpw $0x17,OLDSS(%esp)
	jne 3f
#	movl singal(%eax),%ebx
#	movl blocked(%eax), %ecx
#	notl %ecx
#	andl %ebx,%ecx
#	bsfl %ecx,%ecx
#	je 3f
#	btrl %ecx,%ebx
#	movl %ebx,signal(%eax)
#	incl %ecx
#	pushl %ecx
#	call _do_signal
	popl %eax
3:	popl %eax
	popl %ebx
	popl %ecx
	popl %edx
	pop %fs
	pop %es
	pop %ds
	iret

.align 2
timer_interrupt:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax
	movl $0x10,%eax
	mov %ax, %ds
	mov %ax, %es
	movl $0x17,%eax
	mov %ax, %fs
	incl jiffies
	movb $0x20 ,%al
	outb %al,$0x20
	movl CS(%esp),%eax
	andl $3,%eax
	pushl %eax
	call do_timer
	addl $4,%esp
	jmp ret_from_sys_call


	.align 2
#sys_execve:
#	lea EIP(%esp),%eax
#	pushl %eax
#	call do_execve
#	addl $4,%esp
#	ret

	.align 2
sys_fork:
	call find_empty_process
	testl %eax,%eax
	js 1f
	push %gs
	pushl %esi
	pushl %edi
	pushl %ebp
	pushl %eax
	call copy_process
	addl $20,%esp
1:	ret

