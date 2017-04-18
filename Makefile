OBJECTS = boot.o kernel.o fb.o 
CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld  -ffreestanding -O2 -nostdlib -lgcc
AS = i686-elf-as
all: wos.iso

wos.iso: wos.elf
	rm -fr isodir
	mkdir -p isodir/boot/grub
	cp wos.elf isodir/boot/wos.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o wos.iso isodir


wos.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o wos.elf
	
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
%.o:%.s
	$(AS) $< -o $@
	
clean:
	rm -fr *.o wos.elf wos.iso isodir
