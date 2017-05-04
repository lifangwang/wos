#include <stddef.h>
#include <stdint.h>
#include "misc.h"

void print_test()
{
	int a=0;
	printf("a is %d\n",a);
	printf("addr of a is 0x%p\n", &a);
}


void trap_test()
{
	//divide by 0
	volatile int a = 1/0;
}
