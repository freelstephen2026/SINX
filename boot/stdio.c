CODE32

#include "asm.h"
#include "stdio.h"

unsigned char* vga_buf = (unsigned char*)0xb8000;

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const unsigned char DEFAULT_COLOR = 0x7;

unsigned int pos_x = 0, pos_y = 0;

const char HEXDIGITS[] = "0123456789abcdef";

static void setchr(int x, int y, char c)
{
	vga_buf[2 * (x + y * SCREEN_WIDTH)] = c;
}
static char readchr(int x, int y)
{
	return vga_buf[2 * (x + y * SCREEN_WIDTH)];
}
static void setcolor(int x, int y, unsigned char color)
{
	vga_buf[2 * (x + y * SCREEN_WIDTH) + 1] = color;
}
static unsigned char readcolor(int x, int y)
{
	return (unsigned char)vga_buf[2 * (x + y * SCREEN_WIDTH) + 1];
}

#define SETCHR(_x, _y, _chr)    	vga_buf[2 * (_y * SCREEN_WIDTH + _x)] = _chr
#define GETCHR(_x, _y)              vga_buf[2 * (_y * SCREEN_WIDTH + _x)]
#define SETCOLOR(_x, _y, _color)    vga_buf[2 * (_y * SCREEN_WIDTH + _x) + 1] = _color
#define GETCOLOR(_x, _y)			vga_buf[2 * (_y * SCREEN_WIDTH + _x) + 1]

static void scrollback(unsigned int lines);

void set_cursor(unsigned short x, unsigned short y)
{
    unsigned int pos = y * SCREEN_WIDTH + x;

    asm_outb(0x3D4, 0x0F);
    asm_outb(0x3D5, (unsigned char)(pos & 0xFF));
    asm_outb(0x3D4, 0x0E);
    asm_outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void flush_screen(void)
{
	for (unsigned int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (unsigned int x = 0; x < SCREEN_WIDTH; x++)
		{
			setchr(x, y, 0);
			setcolor(x, y, DEFAULT_COLOR);
		}
	}
	pos_x = 0;
	pos_y = 0;
	set_cursor(0, 0);
}

void puts(char* _buf)
{
	while (*_buf)
	{
		putchar(*_buf);
		_buf++;
	}
	putchar('\n');
}

void putchar(char c)
{
    switch (c)
    {
        case '\n':
            pos_x = 0;
            pos_y++;
            break;
    
        case '\t':
            for (int i = 0; i < 4 - (pos_x % 4); i++)
            {
                SETCHR(pos_x, pos_y, ' ');
                pos_x++;
                break;
            }
            break;

        case '\r':
            pos_x = 0;
            break;

        default:
            vga_buf[2 * (pos_y * SCREEN_WIDTH + pos_x)] = c;
            pos_x++;
            break;
    }

    if (pos_x >= SCREEN_WIDTH)
    {
        pos_y++;
        pos_x = 0;
    }
    if (pos_y >= SCREEN_HEIGHT)
        scrollback(1);

    set_cursor(pos_x, pos_y);
}

static void scrollback(unsigned int lines)
{
    for (unsigned int y = lines; y < SCREEN_HEIGHT; y++)
	{
        for (unsigned int x = 0; x < SCREEN_WIDTH; x++)
        {
            // original: putchr(x, y - lines, getchr(x, y));
			setchr(x, y - lines, readchr(x, y));
            // original: putcolor(x, y - lines, getcolor(x, y));
            setcolor(x, y - lines, readcolor(x, y));
        }
	}

    for (unsigned int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
	{
        for (unsigned int x = 0; x < SCREEN_WIDTH; x++)
        {
            setchr(x, y, 0);
			setcolor(x, y, DEFAULT_COLOR);
        }
	}

    pos_y -= lines;
}

void print_uint(unsigned int num, unsigned int radix)
{
	char buf[32];
	unsigned int pos = 0;

	do
	{
		unsigned int rem = num % radix;
		num /= radix;
		putchar(HEXDIGITS[rem]);
	} while (num);
}

void printf(char *c, ...)
{
    int num;
    __builtin_va_list arg;
    __builtin_va_start(arg, c);
    char outbuf[2048];
    int i;
    char *ch;
    double db_val;
    unsigned int uint_val;
    while(*c!='\0'){
        if(*c=='%'){
            c++;
            switch(*c){
				case 'd':
					num = __builtin_va_arg(arg, int);
					if (num < 0)
					{
						putchar('-');
						num *= -1;
						print_uint(num, 10);
						break;
					}
					print_uint(num, 10);
					break;
				case 'i':
					num = __builtin_va_arg(arg, unsigned int);
					print_uint(num, 10);
					break;
                case 'c':
                case 'C':
                    num = __builtin_va_arg(arg, int);
                    putchar(num);
                	break;
                case 's':
                case 'S':
                    ch = __builtin_va_arg(arg, char*);
                    while(*ch!='\0'){
                        putchar(*ch++);
                    }
                break;
            }
        }
		else
            putchar(*c);
        c++;
    }
    __builtin_va_end(arg);
}