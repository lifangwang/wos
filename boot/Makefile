BUILD_DIR := ../build/boot

ASM_SRC := $(wildcard *.asm)
OBJECTS += $(patsubst %.asm,$(BUILD_DIR)/%.o,$(ASM_SRC))
NASM = nasm
OS := ../build/os/kernel.img

all:boot.img

boot.img: $(OBJECTS)
	@echo $(ASM_SRC)
	@echo $(OBJECTS)
	cat $(BUILD_DIR)/bootsec.o   >$(BUILD_DIR)/boot.img
	cat $(BUILD_DIR)/setup.o  >>$(BUILD_DIR)/boot.img



$(BUILD_DIR)/%.o:%.asm
	KERNEL_SIZE=`ls -l ../build/os/kernel.img | awk '{ print $$5 }'`; \
	KERNEL_SIZE=`expr \( $$KERNEL_SIZE + 512 \) / 512`;	\
	echo "Kernel size(sectors) = " $$KERNEL_SIZE;	\
	$(NASM) -DKERNEL_SIZE=$$KERNEL_SIZE $< -o $@


clean:
	rm -fr $(BUILD_DIR)/* 
	
	
