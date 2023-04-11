section .text
[bits 32]
global asm_outb
asm_outb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

global asm_inb
asm_inb:
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret

global freeze
freeze:
    cli
    hlt
    jmp freeze ; this may not be necessary, but is safe