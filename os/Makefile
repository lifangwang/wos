BUILD_DIR := ../build/os
C_SRC := $(wildcard *.c)
ASM_SRC := $(wildcard *.s)
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRC))
OBJECTS_head := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SRC))
HEAD := $(BUILD_DIR)/head.o
OBJECTS_trap :=  $(BUILD_DIR)/traps_asm.o
OBJECTS_syscall :=  $(BUILD_DIR)/system_call.o
CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I../include
LDFLAGS = -T linker.ld  -ffreestanding -O2 -nostdlib -lgcc  
#LDFLAGS = -T os.ld  -ffreestanding -O2 -nostdlib -lgcc
AS = i686-elf-as
#AS = nasm
all:kernel.img


wos.elf: $(OBJECTS)
	@echo $(SRC)
	@echo $(OBJECTS)
	#ld -m elf_i386 -nmagic -Tos.ld $(OBJECTS) -o $(BUILD_DIR)/kernel.img
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/kernel.elf 
	objcopy  -O binary -R .note -R .comment $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.img

kernel.img:$(OBJECTS) $(OBJECTS_head)
	ld -m elf_i386 -nmagic -Tlinker.ld -M $(HEAD) $(OBJECTS) $(OBJECTS_trap) $(OBJECTS_syscall) -o $(BUILD_DIR)/kernel.elf > $(BUILD_DIR)/kernel.map
	objcopy  -O binary -R .note -R .comment $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.img
	
	
$(BUILD_DIR)/%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)/%.o:%.s
	$(AS) $< -o $@
	
clean:
	rm -fr $(BUILD_DIR)/*
