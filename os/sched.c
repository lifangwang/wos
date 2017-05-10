#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "misc.h"
#include "io.h"
#include "os.h"
#include "mm.h"
#include "sched.h"

union task_union {
		struct task_struct task;
		char stack[PAGE_SIZE];
};

static union task_union init_task = {INIT_TASK,};

long volatile jiffies=0;
long startup_time=0;
struct task_struct *current = &(init_task.task);
struct task_struct *last_task_used_math = NULL;

struct task_struct * task[NR_TASKS] = {&(init_task.task), };

long user_stack [ PAGE_SIZE>>2 ] ;

struct {
		long * a;
		short b;
} stack_start = { & user_stack [PAGE_SIZE>>2] , 0x10 };

extern int system_call(void);
extern int timer_interrupt(void);
/*
 * switch tasks
 */
void schedule(void)
{

	int i,next,c;
	struct task_struct ** p;
	printf("schedule start\n");
	while(1){
		c = -1;
		next = 0;
		i = NR_TASKS;
		p = &task[NR_TASKS];
		while(--i){
			if(! *--p)
				continue;
			printf("p is 0x%x\n", *p);
			if((*p)->state == TASK_RUNNING &&(*p)->counter >c) //find counter most task
				c = (*p)->counter,next = i;
		}
		if(c) break;
		for(p = &LAST_TASK;p>&FIRST_TASK;--p){
			if(*p)
				(*p)->counter = ((*p)->counter>>1)+(*p)->priority;
		}
	}
	printf("schedule to %d\n", next);
	//switch_to(next);
}


void do_timer(long cpl)
{
	//printf("do_timer %d, current:0x%p, counter:%d\n", cpl, current,current->counter);
	if(cpl)
		current->utime++;
	else
		current->stime++;
	if((--current->counter) >0) return; //time slice not finished
	current->counter = 0;
	if(!cpl)
		return;
	schedule();
}
#define LATCH (1193180/HZ)

void sched_init()
{
	int i;
	struct desc_struct *p;
	set_tss_desc(_gdt+FIRST_TSS_ENTRY, &(init_task.task.tss));
	set_ldt_desc(_gdt+FIRST_LDT_ENTRY, &(init_task.task.ldt));
	p = _gdt+2+FIRST_TSS_ENTRY;
	for(i=1; i<NR_TASKS; i++){
		task[i] = NULL;
		p->a = p->b = 0;
		p++;
		p->a = p->b = 0;
		p++;
		

	}
	/* Clear NT, so that we won't have troubles with that later on */
		__asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");

	ltr(0);
	lldt(0);
	//outb_p(0x36,0x43);		/* binary, mode 3, LSB/MSB, ch 0 */
	//outb_p(LATCH & 0xff , 0x40);	/* LSB */
	//outb(LATCH >> 8 , 0x40);	/* MSB */
	//set_intr_gate(0x20,&timer_interrupt);
	//outb(inb_p(0x21)&~0x01,0x21);

	set_system_gate(0x80, &system_call);
}



