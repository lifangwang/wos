#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include "io.h"
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
struct vga_info{
	uint16_t *fb;
	uint8_t bg_color;
	size_t width;
	size_t height;
	uint8_t cursor_x;
	uint8_t cursor_y;
}__attribute__((packed)) vga;



void fb_move_cursor(unsigned short pos)
{
	outb( FB_HIGH_BYTE_COMMAND,FB_COMMAND_PORT);
	outb( ((pos>>8) & 0x00ff),FB_DATA_PORT);
	outb( FB_LOW_BYTE_COMMAND,FB_COMMAND_PORT);
	outb( pos&0x00ff,FB_DATA_PORT);
}

/**
get fb color
*/
static inline uint8_t fb_color_entry(uint8_t fg, uint16_t bg)
{
	uint8_t color = bg<< 4 | fg;
	return color;
	
}

    /** fb_write_cell:
     *  Writes a character with the given foreground and background to position i
     *  in the framebuffer.
     *
     *  @param i  The location in the framebuffer
     *  @param c  The character
     *  @param fg The foreground color
     *  @param bg The background color
     */
static inline   void fb_write_cell(size_t i, char c, uint8_t color_info)
{
        vga.fb[i] = c | color_info <<8;
        
}

static inline void clear_line(uint8_t row_index)
{
	size_t clear_index = row_index*vga.width;
	size_t clear_last = vga.height * vga.width;
	for(; clear_index <clear_last  ; clear_index++)
		fb_write_cell(clear_index, ' ', vga.bg_color);
	
}

/**
scroll  line
*/
static void fb_scroll(size_t line)
{
/*	
	
	size_t i=0;
	volatile uint16_t *src = vga.fb+line*vga.width;
	size_t copy_len = (vga.height-line)*vga.width;
	for(i=0; i<copy_len; i++){
			vga.fb[i] = src[i];
	}

	//clear  lines
	clear_line(vga.height-line);
	
*/	
}
/**
write data to fb
*/

int fb_write(char* data, size_t data_len)
{
	char out_data;
	uint8_t is_display = 1;
	uint16_t cursor_pos = 0;
	
	for(size_t i=0; i<data_len; i++)
	{
		out_data = data[i];
		switch(out_data){
			case '\n':
			case '\r':
			//new line
				vga.cursor_y +=1;
				vga.cursor_x = 0;
				is_display = 0;
				break;
			case '\t':
				vga.cursor_x += 4;
				is_display = 0;
				break;
			default:
				vga.cursor_x += 1;
				is_display = 1;
				break;
				
		}
		if(vga.cursor_x == vga.width){
			vga.cursor_y += 1;
			vga.cursor_x = 0;
		}
		if(vga.cursor_y == vga.height){	
			fb_scroll(1);
			vga.cursor_y = vga.height-1;
		}
		cursor_pos = vga.cursor_y * vga.width + vga.cursor_x;
		fb_move_cursor(cursor_pos);
		
		if(is_display){
			fb_write_cell(cursor_pos, data[i], vga.bg_color);
		}
	}
	
	return data_len;
}

static void fb_clear()
{
	
	size_t fblen = vga.width * vga.height;
	for(size_t i=0; i<fblen ; i++)
		fb_write_cell(i, ' ', vga.bg_color);
	vga.cursor_x = 0; //position for cursor
	vga.cursor_y = 0;
}

/**
init fb
*/
void fb_init()
{
	vga.fb = (uint16_t *) 0x000B8000;
	vga.width = 80;
	vga.height = 25;
	vga.bg_color = 8<<4 | 2;
	
	  
	//clear all the chars
	fb_clear();

	
}
