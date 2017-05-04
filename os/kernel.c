/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#include <stddef.h>
#include <stdint.h>
#include "misc.h"
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 

 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
extern void mem_init();
extern void fb_init();
extern void trap_init();


//test functions
extern void print_test();
void main(void) {
	/* Initialize terminal interface */
	char* info = "hello kernel\nhello\tagain\n";
	fb_init();
	/* Newline support is left as an exercise. */
	printf(info);
	print_test();
	mem_init();
	trap_init();
	trap_test();
	//volatile int test = 1/0;
	for(;;){
	}
}
