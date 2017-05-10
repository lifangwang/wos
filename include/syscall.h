#ifndef SYSCALL_H_
#define SYSCALL_H_

#define __NR_setup	0	/* used only by init, to get system going */
#define __NR_exit	1
#define __NR_fork	2
#define __NR_read	3
#define __NR_write	4
#define __NR_open	5
#define __NR_close	6
#define __NR_waitpid	7
#define __NR_creat	8
#define __NR_link	9
#define __NR_unlink	10
#define __NR_execve	11



#define _syscall0(type,name) \
	type name(void) \
{ \
	long __res; \
	__asm__ volatile ("int $0x80" \
				: "=a" (__res) \
				: "0" (__NR_##name)); \
	if (__res >= 0) \
		return (type) __res; \
	return -1; \
}


typedef int (*fn_ptr)();


extern int sys_setup();
extern int sys_exit();
extern int sys_fork();
extern int sys_read();
extern int sys_write();
extern int sys_open();
extern int sys_close();
extern int sys_waitpid();
extern int sys_creat();
extern int sys_link();
extern int sys_unlink();
extern int sys_execve();

fn_ptr sys_call_table[] = { sys_setup, sys_exit, sys_fork, sys_read,
	sys_write, sys_open, sys_close, sys_waitpid, sys_creat, sys_link,
	sys_unlink, sys_execve};

#endif

