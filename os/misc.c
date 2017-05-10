#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "fb.h"
static char log_buf[1024];
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void puts(char* info)
{
	fb_write(info, strlen(info));
		
}

#define do_div(n,base) ({ \
		int __res; \
		__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
		__res; })

 char* number(char* buf, int num, int base)
{
	char c,sign,tmp[36];
	const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i = 0;
	if(num == 0)
		tmp[i++] = '0';
	else{
		while(num)
			tmp[i++] = digits[do_div(num,base)];
	}
	while(i--)
		*buf++ = tmp[i];
	return buf;

}
int vsprintf(const char* fmt,va_list args)
{
	char* buf_ptr, *s;
	for(buf_ptr = log_buf; *fmt ; fmt++){
		if(*fmt != '%'){
			*buf_ptr++ = *fmt;
			continue;
		}
		++fmt;
		switch(*fmt){
			case 'c':
				*buf_ptr++ = (unsigned char) va_arg(args,int);
				break;
			case 's':
				s = (char*)va_arg(args,char *);
				while(*s)
					*buf_ptr++ = *s++;
				break;
			case 'o':
				buf_ptr = number(buf_ptr,va_arg(args, unsigned long),8);
				break;
			case 'x':
			case 'X':
				buf_ptr = number(buf_ptr, va_arg(args,unsigned long), 16);
				break;
			case 'p':
				buf_ptr = number(buf_ptr, va_arg(args,void*), 16);
				break;
			case 'd':
			case 'u':
				buf_ptr = number(buf_ptr, va_arg(args,unsigned long), 10);
				break;
			default:
				if(*fmt !='%')
					*buf_ptr++ = '%';
				if(*fmt)
					*buf_ptr++ = *fmt;
				else
					--fmt;
				break;


		}
		
	}
	*buf_ptr = 0;
	puts(log_buf);
	return buf_ptr - log_buf;

}

void printf(const char* fmt, ...)
{
	va_list args;
	va_start(args,fmt);
	vsprintf(fmt, args);
	va_end(args);
}
