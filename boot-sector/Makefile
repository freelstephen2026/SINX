all: stage1

stage1: build/stage1.bin

build/stage1.bin: boot-sector/boot.asm
	nasm -f bin -o $@ $^
	@echo "--> Created  stage1.bin"

clean:
	@rm -f build/stage1.bin