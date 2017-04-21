bits 16
BOOTSEG EQU 0X7C0
INITSEG EQU 0X9000
SETUPSEG EQU 0X9020
SYSSEG EQU 0X1000
CYLS equ 10
SETUPLEN equ 1

start:
	jmp BOOTSEG:boot
msg	db "hello kernel", 0ah, 0dh, 0h
err	db "error", 0ah, 0dh, 0h
msg1	db "load setup ok", 0ah, 0dh, 0h
msg2	db "load system ok", 0ah, 0dh, 0h

boot:
	cli
	;mov	ax,cs
	;mov 	ds,ax
	;mov	es,ax
	;mov	ss,ax
	;mov	sp,0x0400
	
	;mov	ah,0x03		;read cursor pos
	;xor	bh,bh
	;int	0x10
	
	;mov	cx,15
	;mov	bx,0x0007		; page 0, attribute 7 (normal)
	;mov	bp,msg
	;mov	ax,0x1301		; write string, move cursor
	;int	0x10
	
	cld
	mov	ax,cs
	mov	ds,ax
	mov	ax,INITSEG
	mov	es,ax
	mov	cx,256
	sub	si,si
	sub	di,di
	rep	movsw

	nop
	nop
	jmp  dword INITSEG:go		;goto INITSEG
go:
	mov ax,cs
	mov ds,ax
	mov es,ax	
	mov ss,ax
	mov sp, 0xff00 		;put stack at 0x9ff00



load_setup:
	mov	ax,INITSEG		; copy sector 2 to 0x9000:0
	mov	es,ax
	mov	dx,0x0000		; drive 0, head 0
	mov	cx,0x0002		; sector 2, track 0
	mov	bx,0x0200		; address = 512, in INITSEG
	mov	ax,0x0200+SETUPLEN	; service 2, nr of sectors
	int	0x13			; read it
	jnc	ok_load_setup		; ok - continue
	mov	dx,0x0000		;
	mov	ax,0x0000		; reset the diskette
	int	0x13
	jmp	load_setup

ok_load_setup:
	mov 	ax,cs		;print ok msg
	mov	es,ax
	mov	ah,0x03		;read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,15
	mov	bx,0x0007		; page 0, attribute 7 (normal)
	mov	bp,msg1
	mov	ax,0x1301		; write string, move cursor
	int	0x10

load_system:
	
	mov 	ax,SYSSEG
	
	mov	es,ax
	mov	dx,0x0000		; drive 0, head 0
	mov	cx,0x0003		; sector 3, track 0
	mov	bx,0x0000		; address = 512, in INITSEG
	mov	ax,0x0200+KERNEL_SIZE	; service 2, nr of sectors
	int	0x13			; rea

	jnc	ok_load_system		; ok - continue
	mov	dx,0x0000		;
	mov	ax,0x0000		; reset the diskette
	int	0x13
	jmp	load_system

ok_load_system:
	mov 	ax,cs		;print ok msg
	mov	es,ax
	mov	ah,0x03		;read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,15
	mov	bx,0x0007		; page 0, attribute 7 (normal)
	mov	bp,msg2
	mov	ax,0x1301		; write string, move cursor
	int	0x10
	jmp	dword SETUPSEG:0X0	

error:
	mov	ah,0x03		;read cursor pos
	xor	bh,bh
	int	0x10
	
	mov	cx,5
	mov	bx,0x0007		; page 0, attribute 7 (normal)
	mov	bp,err
	mov	ax,0x1301		; write string, move cursor
	int	0x10
error1:
	hlt
	jmp error1

gdt:
	db	0,0,0,0		; dummy

	dw	0x07FF		; 8Mb - limit=2047 (2048*4096=8Mb)
	dw	0x0000		; base address=0
	dw	0x9A00		; code read/exec
	dw	0x00C0		; granularity=4096, 386

	dw	0x07FF		; 8Mb - limit=2047 (2048*4096=8Mb)
	dw	0x0000		; base address=0
	dw	0x9200		; data read/write
	dw	0x00C0		; granularity=4096, 386

idt_48:
	dw	0			; idt limit=0
	dw	0,0			; idt base=0L

gdt_48:
	dw	0x800		; gdt limit=2048, 256 GDT entries
	dw	gdt,0x9		; gdt base = 0X9xxxx



times 510-($-$$) db 0
dw 0xaa55
