#ifndef _BOOT_ASM_H
#define _BOOT_ASM_H

void __attribute__((cdecl)) asm_outb(unsigned short port, unsigned short value);
unsigned char __attribute__((cdecl)) asm_inb(unsigned short port);
void freeze(void);

#endif