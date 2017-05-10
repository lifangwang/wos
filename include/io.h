#ifndef IO_H
#define IO_H

/**
outb

void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t data);
uint16_t inw(uint16_t port);
*/
/* Basic port I/O */
static inline void outb(uint8_t v, uint16_t port)
{
        asm volatile("outb %0,%1" : : "a" (v), "dN" (port));
}
static inline uint8_t inb(uint16_t port)
{
        uint8_t v;
        asm volatile("inb %1,%0" : "=a" (v) : "dN" (port));
        return v;
}

static inline void outw(uint16_t v, uint16_t port)
{
        asm volatile("outw %0,%1" : : "a" (v), "dN" (port));
}
static inline uint16_t inw(uint16_t port)
{
        uint16_t v;
        asm volatile("inw %1,%0" : "=a" (v) : "dN" (port));
        return v;
}

static inline void outl(uint32_t v, uint16_t port)
{
        asm volatile("outl %0,%1" : : "a" (v), "dN" (port));
}
static inline uint32_t inl(uint16_t port)
{
        uint32_t v;
        asm volatile("inl %1,%0" : "=a" (v) : "dN" (port));
        return v;
}
#define outb_p(value,port) \
	__asm__ ("outb %%al,%%dx\n" \
					"\tjmp 1f\n" \
					"1:\tjmp 1f\n" \
					"1:"::"a" (value),"d" (port))

#define inb_p(port) ({ \
		unsigned char _v; \
		__asm__ volatile ("inb %%dx,%%al\n" \
				"\tjmp 1f\n" \
				"1:\tjmp 1f\n" \
				"1:":"=a" (_v):"d" (port)); \
		_v; \
		})


#endif
