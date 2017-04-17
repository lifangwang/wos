OBJECTS = boot.o kernel.o fb.o io.o
CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld  -ffreestanding -O2 -nostdlib -lgcc
AS = i686-elf-as
all: wos.elf


wos.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o wos.elf
	
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
%.o:%.s
	$(AS) $< -o $@
	
clean:
	rm -fr *.o wos.elf 