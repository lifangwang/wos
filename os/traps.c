#include <stdint.h>
#include <stddef.h>
#include "misc.h"
#include "os.h"
#include "sched.h"

void divide_error();
void debug();
void nmi();
void int3(void);
void overflow(void);
void bounds(void);
void invalid_op(void);
void device_not_available(void);
void double_fault(void);
void coprocessor_segment_overrun(void);
void invalid_TSS(void);
void segment_not_present(void);
void stack_segment(void);
void general_protection(void);
void page_fault(void);
void coprocessor_error(void);
void reserved(void);
#define get_seg_byte(seg,addr) ({ \
		register char __res; \
		__asm__("push %%fs;mov %%ax,%%fs;movb %%fs:%2,%%al;pop %%fs" \
				:"=a" (__res):"0" (seg),"m" (*(addr))); \
		__res;})

#define get_seg_long(seg,addr) ({ \
		register unsigned long __res; \
		__asm__("push %%fs;mov %%ax,%%fs;movl %%fs:%2,%%eax;pop %%fs" \
				:"=a" (__res):"0" (seg),"m" (*(addr))); \
		__res;})

#define _fs() ({ \
		register unsigned short __res; \
		__asm__("mov %%fs,%%ax":"=a" (__res):); \
		__res;})

static void die(char* info, long esp_ptr, long nr)
{
	long* esp = (long*)esp_ptr;
	int i;
	printf("%s:%d\n",info, nr);
	printf("eip[0x%x:0x%x],eflags[0x%x],esp[0x%x:0x%x]\n",esp[1],esp[0],esp[2],esp[4],esp[3]);
	printf("fs: 0x%x\n",_fs());
	printf("base: %p, limit: %p\n",get_base(current->ldt[1]),get_limit(0x17));
	if (esp[4] == 0x17) {
		printf("Stack: ");
		for (i=0;i<4;i++)
			printf("%p ",get_seg_long(0x17,i+(long *)esp[3]));
			printf("\n");
	}
	str(i);
	printf("Pid: %d, process nr: %d\n\r",current->pid,0xffff & i);
	for(i=0;i<10;i++)
		printf("0x%x ",0xff & get_seg_byte(esp[1],(i+(char *)esp[0])));
	printf("\n\r");
	printf("g1 is 0x%x==0x%x\n", _gdt[1].a, _gdt[1].b); //cs
	printf("g2 0 is 0x%x==0x%x\n", _gdt[2].a, _gdt[2].b);//ds
	printf("g3 0 is 0x%x==0x%x\n", _gdt[3].a, _gdt[3].b);//syscall
	printf("g4 0 is 0x%x==0x%x\n", _gdt[4].a, _gdt[4].b);//tss0
	printf("g5 0 is 0x%x==0x%x\n", _gdt[5].a, _gdt[5].b);//ldt0
	printf("g6 0 is 0x%x==0x%x\n", _gdt[6].a, _gdt[6].b);//tss1
	printf("g7 0 is 0x%x==0x%x\n", _gdt[7].a, _gdt[7].b);//ldt1

}
void trap_init()
{
//	printf("trap init start\n");
	set_trap_gate(0, &divide_error);
	set_trap_gate(1, &debug);
	set_trap_gate(2, &nmi);
	set_system_gate(3, &int3);
	set_system_gate(4,&overflow);
	set_system_gate(5,&bounds);
	set_trap_gate(6,&invalid_op);
	//set_trap_gate(7,&device_not_available);
	set_trap_gate(7,&reserved);
	set_trap_gate(8,&double_fault);
	set_trap_gate(9,&coprocessor_segment_overrun);
	set_trap_gate(10,&invalid_TSS);
	set_trap_gate(11,&segment_not_present);
	set_trap_gate(12,&stack_segment);
	set_trap_gate(13,&general_protection);
	//set_trap_gate(14,&page_fault);
	set_trap_gate(14,&reserved);
	set_trap_gate(15,&reserved);
	//set_trap_gate(16,&coprocessor_error);
	set_trap_gate(16,&reserved);
	/*
	int i;
	for(i=17;i<48;i++){
		set_trap_gate(i,&reserved);
	}*/
	//set_trap_gate(45,&irq13);	
	set_trap_gate(45,&reserved);



//	printf("trap init end\n");
//	printf("gate 0 is 0x%x==0x%x\n", _idt[0].a, _idt[0].b);
//	printf("gate 3 is 0x%x==0x%x\n", _idt[3].a, _idt[3].b);
}

void do_divide_error(long esp, long error_code)
{
	die("do_divide_error",esp, error_code);
}
void do_nmi(long esp, long error_code)
{
	printf("do_nmi");
}
void do_int3(long esp, long error_code)
{
	printf("do_int3");
}

void do_double_fault(long esp, long error_code)
{
		die("double fault",esp,error_code);
}

void do_general_protection(long esp, long error_code)
{
		die("general protection",esp,error_code);
}


void do_debug(long esp, long error_code)
{
		die("debug",esp,error_code);
}

void do_overflow(long esp, long error_code)
{
		die("overflow",esp,error_code);
}

void do_bounds(long esp, long error_code)
{
		die("bounds",esp,error_code);
}

void do_invalid_op(long esp, long error_code)
{
		die("invalid operand",esp,error_code);
}

void do_device_not_available(long esp, long error_code)
{
		die("device not available",esp,error_code);
}

void do_coprocessor_segment_overrun(long esp, long error_code)
{
		die("coprocessor segment overrun",esp,error_code);
}

void do_invalid_TSS(long esp,long error_code)
{
		die("invalid TSS",esp,error_code);
}

void do_segment_not_present(long esp,long error_code)
{
		die("segment not present",esp,error_code);
}

void do_stack_segment(long esp,long error_code)
{
		die("stack segment",esp,error_code);
}

void do_coprocessor_error(long esp, long error_code)
{
			die("coprocessor error",esp,error_code);
}

void do_reserved(long esp, long error_code)
{
		die("reserved (15,17-47) error",esp,error_code);
}
