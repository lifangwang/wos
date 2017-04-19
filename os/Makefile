BUILD_DIR := ../build/os
C_SRC := $(wildcard *.c)
ASM_SRC := $(wildcard *.s)
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRC))
OBJECTS += $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SRC))
CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I../include
LDFLAGS = -T linker.ld  -ffreestanding -O2 -nostdlib -lgcc
AS = i686-elf-as
all:wos.elf


wos.elf: $(OBJECTS)
	@echo $(SRC)
	@echo $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/wos.elf
	
$(BUILD_DIR)/%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)/%.o:%.s
	$(AS) $< -o $@
	
clean:
	rm -fr $(BUILD_DIR)/*
