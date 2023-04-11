#ifndef _BOOT_STDIO_H
#define _BOOT_STDIO_H

void set_cursor(unsigned short x, unsigned short y);
void putchar(char c);
void flush_screen(void);
void puts(char*);
void printf(char *c, ...);

#endif