BUILD_DIR = build
OS := $(BUILD_DIR)/os/kernel.img
BOOT := $(BUILD_DIR)/boot/boot.img
LDFLAGS = -T linker.ld  -ffreestanding -O2 -nostdlib -lgcc

all: wos.iso

.PHONY: wos.iso   bootloader

kernel:
	make -C os
	
bootloader: kernel
	make -C boot 


wos.iso: bootloader kernel
	dd if=/dev/zero of=disk.img bs=512 count=2880
	dd conv=notrunc if=$(BOOT) of=disk.img bs=512 count=$$(( $(shell stat --printf="%s" $(BOOT))/512))  seek=0
	dd conv=notrunc if=$(OS) of=disk.img bs=512 count=$$(( $(shell stat --printf="%s" $(OS))/512 + 1)) seek=$$(( $(shell stat --printf="%s" $(BOOT))/512)) 
clean:
	make -C os clean
	make -C boot clean
	rm -fr disk.img
	
	
