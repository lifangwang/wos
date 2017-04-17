.global outb

outb:
	push %dx
	mov 4(%esp), %dx
	movb 6(%esp), %al
	outb %al,%dx
	pop %dx
	ret
