#include <stddef.h>
#include <stdint.h>
#include "os.h"
#include "sched.h"
#include "mm.h"
#include "misc.h"
long last_pid = 0;
int find_empty_process(void)
{
	int i;
repeat:
	if((++last_pid) <0) last_pid = 1;
	for(i=0;i<NR_TASKS; i++)
		if(task[i] && task[i]->pid == last_pid) goto repeat;

	for(i=0; i<NR_TASKS; i++)
		if(!task[i])
			return i;
	return -1;
}

int copy_mem(int nr, struct task_struct* p)
{
	unsigned long old_data_base, new_data_base,data_limit;
	unsigned long old_code_base, new_code_base, code_limit;
	code_limit = get_limit(0x0f);
	data_limit = get_limit(0x17);
	old_code_base = get_base(current->ldt[1]);
	old_data_base = get_base(current->ldt[2]);
	if(old_data_base != old_code_base)
		printf("i&d not the same");
	if(data_limit < code_limit)
		printf("base limit");
	new_data_base = new_code_base = nr*0x4000000;
	//printf("copy mem:nr:%d,ocode:0x%x,ndata:0x%x\n", nr, old_code_base, new_code_base);
	p->start_code = new_code_base;
	set_base(p->ldt[1],new_code_base);
	set_base(p->ldt[2], new_data_base);
	if(copy_page_tables(old_data_base,new_data_base,data_limit)){
		free_page_tables(new_data_base, data_limit);
		return -2;
	}
	return 0;
}
int copy_process(int nr, long ebp, long edi,long esi,long gs,long none,long ebx,
		long ecx,long edx,long fs,long es,long ds,long eip,long cs,
		long eflags,long esp,long ss)
{
	struct task_struct *p;

	p = (struct task_struct*) get_free_page();
	if(!p)
		return -1;
	task[nr] = p;
	*p = *current; //copy task things
	p->state = TASK_UNINTERRUPTIBLE;
	p->pid = last_pid;
	p->father = current->pid;
	p->counter = p->priority;
	//p->signal = 0;
	p->alarm = 0;
	p->leader = 0;
	p->utime = p->stime = 0;
	p->cutime = p->cstime = 0;
	p->start_time = jiffies;
	p->tss.back_link = 0;
	p->tss.esp0 = PAGE_SIZE + (long)p;
	p->tss.ss0 = 0x10;//kernel ds?
	p->tss.eip = eip;
	p->tss.eflags = eflags;
	p->tss.eax = 0;
	p->tss.ebx = ebx;
	p->tss.ecx = ecx;
	p->tss.edx = edx;

	p->tss.esp = esp;

	p->tss.ebp = ebp;

	p->tss.esi = esi;
	p->tss.edi = edi;
	p->tss.es = es & 0xffff;
	p->tss.cs = cs & 0xffff;
	p->tss.ss = ss & 0xffff;
	p->tss.ds = ds & 0xffff;
	p->tss.fs = fs & 0xffff;
	p->tss.gs = gs & 0xffff;
	p->tss.ldt = _LDT(nr);
	p->tss.trace_bitmap = 0x80000000;
	if(copy_mem(nr, p)){
		task[nr] = NULL;
		free_page((long)p);
		return -1;
	}
	set_tss_desc(_gdt+(nr<<1)+FIRST_TSS_ENTRY,&(p->tss));
	set_ldt_desc(_gdt+(nr<<1)+FIRST_LDT_ENTRY,&(p->ldt));
	p->state = TASK_RUNNING;	/* do this last, just in case */
	//printf("copy process finish pid[%d]\n", last_pid);
	return last_pid;
}


