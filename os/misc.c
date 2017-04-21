#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "fb.h"

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