#include <stdint.h>
#include <stddef.h>
#include "misc.h"
#define EXT_MEM_K (*(unsigned short *)0x90002)
#define LOW_MEM 0X100000
#define PAGING_MEMORY (15*1024*1024)
#define PAGING_PAGES (PAGING_MEMORY >> 12)
#define MAP_NR(addr) ((addr-LOW_MEM)>>12)
#define USED 100
static long main_memory_start = 0;
static long main_memory_end = 0;
static long HIGH_MEM = 0;
static unsigned char mem_map[PAGING_PAGES] = {0,};

void mem_init()
{
	int i;
	long end_memory;
	HIGH_MEM = main_memory_end = (1<<20) + (EXT_MEM_K<<10);
	main_memory_end &= 0xfffff000;
	if(main_memory_end > 16*1024*1024)
		main_memory_end = 16*1024*1024;
	main_memory_start = 1*1024*1024 ; // 1M reserved for memory		
	for(i=0; i<PAGING_PAGES; i++)
		mem_map[i] = USED;
	i = MAP_NR(main_memory_start);
	end_memory = main_memory_end;
	end_memory -= main_memory_start;
	end_memory >>= 12; //change to page frames
	while(end_memory-->0)
		mem_map[i++] = 0;
	puts("mem_init finished\n");	

}

int calc_mem()
{
	int i;
	int free_pages = 0;
	for(i=0;i<PAGING_PAGES;i++){
		if(!mem_map[i])
			free_pages++;
	}
	return free_pages;
}

/**
*	get a free page
*/
unsigned long get_free_page()
{
	int i=MAP_NR(main_memory_start);
	while(i<PAGING_PAGES){
		if(!mem_map[i])
			break;
		i++;
	}
	if(i == PAGING_PAGES)
		return 0;
	mem_map[i] = 1;	
	return main_memory_start + (i<<12);
}
/*
* release a page, where to clear the pte and pg_dir_entry?
*/
int free_page(unsigned long addr)
{
	if(addr < LOW_MEM || addr >= HIGH_MEM)
		return 0;
	addr -= main_memory_start;
	addr >>= 12;
	if(mem_map[addr]--) return 1;  
	else{
		puts("try to free free pages");
		mem_map[addr] = 0;
		return 0;
	}	
}
/*
*map the linear address to page
*/
unsigned long put_page(unsigned long page, unsigned long address)
{
	unsigned long tmp;
	unsigned long *page_table;
	page_table = (unsigned long*)((address >>20)&0xffc); /* pg_dir entry for address*/
	if(*page_table &1)
		page_table = (unsigned long*)(0xfffff000 & *page_table);/* page table address*/
	else{
		if(!(tmp=get_free_page()))
			return 0;
		*page_table = tmp|7; /* pg_dir entry*/
		page_table = (unsigned long*)tmp; /* page table address*/	
	}		
	page_table[(address>>12)&0x3ff] = page | 7; /*pte*/
	return page;
	
}
/*
* get physical page for linear address
*
*/
unsigned long  get_empty_page(unsigned long address)
{
	unsigned long tmp;
	if(!(tmp=get_free_page()) || !put_page(tmp,address)){
		free_page(tmp);
		return 0;
	}
	return tmp;
}

/**
 * page fault handler
 */
void do_no_page(unsigned long error_code, unsigned long address)
{
	int nr[4];
	unsigned long tmp;
	unsigned long page;
	int block ,i;
	address &= 0xfffff000;
	//todo
}
