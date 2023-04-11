#
# Floppy image
#
ARCH?=x86

floppy_image: $(BUILD_DIR)/main_floppy.img

floppy.img: bootloader
	@dd if=/dev/zero of=$@ bs=512 count=2880 > /dev/null
	@mkfs.fat -F12 -n "SINX" $@ > /dev/null
	@dd if=build/stage1.bin of=$@ conv=notrunc > /dev/null
	@mkdir mnt
	@sudo mount -t auto -o loop floppy.img mnt
	@sudo cp build/load mnt
	@sudo umount mnt && rmdir mnt
	@echo "--> Created: floppy.img"

#
# Bootloader
#
bootloader: mkdir_build build/load build/stage1.bin

build/load:
	make -f boot/Makefile SYS_FS=UFS2 ARCH=$(ARCH)

build/stage1.bin:
	@$(MAKE) -f boot-sector/Makefile ARCH=$(ARCH)

#
# Kernel
#
kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin:
	@$(MAKE) -C src/kernel BUILD_DIR=$(abspath $(BUILD_DIR))

#
# Clean
#
clean:
	@rm -rf build floppy.img

mkdir_build:
	mkdir build