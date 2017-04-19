BUILD_DIR = build
OS := $(BUILD_DIR)/os/wos.elf
LDFLAGS = -T linker.ld  -ffreestanding -O2 -nostdlib -lgcc
AS = i686-elf-as
all: wos.iso

.PHONY: wos.iso os

wos.iso: os
	rm -fr isodir
	mkdir -p isodir/boot/grub
	cp $(OS) isodir/boot/wos.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o wos.iso isodir

os:
	make -C os
	
clean:
	make -C os clean
	rm  -fr wos.iso isodir
