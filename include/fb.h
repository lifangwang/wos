#ifndef FB_H
#define FB_H

void fb_move_cursor(unsigned short pos);
int fb_write(char* data, int data_len);
void fb_init();
#endif