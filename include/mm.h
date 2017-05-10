#ifndef MM_H_
#define MM_H_


#define PAGE_SIZE 4096

extern unsigned long get_free_page(void);
extern unsigned long put_page(unsigned long page,unsigned long address);
extern void free_page(unsigned long addr);

extern int copy_page_tables(unsigned long from,unsigned long to,long size);
extern int free_page_tables(unsigned long from,unsigned long size);
#endif
