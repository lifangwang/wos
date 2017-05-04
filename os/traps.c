#include <stdint.h>
#include <stddef.h>
#include "misc.h"
#include "os.h"
void divide_error();
void debug();
void nmi();
static void die(char* info, long esp_ptr, long nr)
{
	long* esp = (long*)esp_ptr;
	int i;
	printf("%s:%d\n",info, nr);
	printf("eip[0x%x:0x%x],eflags[0x%x],esp[0x%x:0x%x]\n",esp[1],esp[0],esp[2],esp[4],esp[3]);


}
void trap_init()
{
	printf("trap init start\n");
	set_trap_gate(0, &divide_error);
	set_trap_gate(1, &debug);
	set_trap_gate(2, &nmi);
	printf("trap init end\n");
	printf("gate 0 is 0x%x==0x%x\n", _idt[0].a, _idt[0].b);
	printf("gate 3 is 0x%x==0x%x\n", _idt[3].a, _idt[3].b);
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
