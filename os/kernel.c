/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#include <stddef.h>
#include <stdint.h>
#include "misc.h"
#include "os.h"
#include "syscall.h"
 
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
extern void sched_init();
static inline _syscall0(int,fork)

//test functions
extern void print_test();

void init(void)
{
	//printf("in init\n");
}

void main(void) {
	int fork_pid = 0;
	/* Initialize terminal interface */
	char* info = "hello kernel\nhello\tagain\n";
	fb_init();
	/* Newline support is left as an exercise. */
	printf(info);
	print_test();
	mem_init();
	trap_init();
	//trap_test();
	//time_init();
	sched_init();
	sti();
	move_to_user_mode();
	puts("usermode?");
	//printf("current in usermode %d\n");
	fork_pid = fork();
	//printf("fork pid is %d\n", fork_pid);
	if(!fork_pid){
		init();//init process starts here
	}
	//printf("fork not 0\n");
	//volatile int test = 1/0;
	for(;;){
	}
}


