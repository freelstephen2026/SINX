TARGET_LINKFLAGS += -T boot/linker.ld -nostdlib

C_MACRO_FLAGS=-D CODE32='__asm__(".code32\n");'
C_FLAGS=$(C_MACRO_FLAGS) -fno-stack-protector -m32 -ffreestanding -nostdlib -fno-pic

THIS_MKFILE_DIR=boot
THIS_MKFILE=boot/Makefile

all: build/load clean_objects

build/load: build/entry.o build/main.o build/stdio.o build/asm.o
	ld $(TARGET_LINKFLAGS) -o $@ $^

build/entry.o: boot/entry.asm
	nasm -felf64 -o $@ $^

build/main.o: boot/main.c
	gcc $(C_FLAGS) -S -o build/main.s $^
	as -o $@ build/main.s
	rm build/main.s
	@echo "compiled boot/main.c"

build/stdio.o: boot/stdio.c
	gcc $(C_FLAGS) -S -o build/stdio.s $^
	as -o $@ build/stdio.s
	rm build/stdio.s
	@echo "compiled --> " $^

build/asm.o: boot/asm.asm
	nasm -felf64 -o $@ $^

clean:
	@rm -f build/load
clean_objects:
	rm -f $(wildcard build/*.o)