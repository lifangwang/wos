#ifndef OS_H_
#define OS_H_
typedef struct desc_struct {
		unsigned long a,b;
} desc_table[256];

extern unsigned long _pg_dir[1024];
extern desc_table _idt,_gdt;


#define sti() __asm__ ("sti"::)
#define cli() __asm__ ("cli"::)
#define nop() __asm__ ("nop"::)

#define iret() __asm__ ("iret"::)

#define _set_gate(gate_addr,type,dpl,addr) \
	__asm__ ("movw %%dx,%%ax\n\t" \
				"movw %0,%%dx\n\t" \
				"movl %%eax,(%1)\n\t" \
				"movl %%edx,(%2)" \
				: \
				: "i" ((short) (0x8000+(dpl<<13)+(type<<8))), \
				"o" (*((char *) (gate_addr))), \
				"o" (*(4+(char *) (gate_addr))), \
				"d" ((char *) (addr)),"a" (0x00080000))

#define set_intr_gate(n,addr) \
		_set_gate(&_idt[n],14,0,addr)

#define set_trap_gate(n,addr) \
		_set_gate(&_idt[n],15,0,addr)

#define set_system_gate(n,addr) \
		_set_gate(&_idt[n],15,3,addr)



#endif
