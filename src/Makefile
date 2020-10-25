BUILD_DIR = ../build

CPP = i686-elf-g++
CPP_FLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

ASM = nasm
ASM_FLAGS = -felf32

LD = i686-elf-gcc
LD_FLAGS = -ffreestanding -O2 -nostdlib

GRUB_TEST = grub-file --is-x86-multiboot $(BUILD_DIR)/sarahos.bin

CPP_FILES = $(shell find . -name '*.cpp')
CPP_OBJS = $(patsubst %.cpp,%.o,$(subst ./,$(BUILD_DIR)/,$(CPP_FILES)))

ASM_FILES = $(shell find . -name '*.asm')
ASM_OBJS = $(patsubst %.asm,%.o,$(subst ./,$(BUILD_DIR)/,$(ASM_FILES)))

.PHONY: clean

$(CPP_OBJS): $(BUILD_DIR)/%.o: ./%.cpp
	@mkdir -p $(BUILD_DIR)/$(dir $<)
	$(CPP) -c -o $@ $< $(CPP_FLAGS)
	@echo ""

$(ASM_OBJS): $(BUILD_DIR)/%.o: ./%.asm
	@mkdir -p $(BUILD_DIR)/$(dir $<)
	$(ASM) $(ASM_FLAGS) -o $@ $<
	@echo ""

kernel: $(CPP_OBJS) $(ASM_OBJS)
	$(LD) -T linker.ld -o $(BUILD_DIR)/sarahos.bin $(LD_FLAGS) $(ASM_OBJS) $(CPP_OBJS) -lgcc

	@if $(GRUB_TEST); then \
		echo "Multiboot check passed."; \
	else \
		echo "Multiboot check failed!"; \
	fi

iso: kernel
	mkdir -p $(BUILD_DIR)/iso/boot/grub

	cp $(BUILD_DIR)/sarahos.bin $(BUILD_DIR)/iso/boot/sarahos.bin
	cp grub.cfg $(BUILD_DIR)/iso/boot/grub/grub.cfg

	grub-mkrescue -o $(BUILD_DIR)/sarahos.iso $(BUILD_DIR)/iso

run: iso
	qemu-system-i386.exe -cdrom $(BUILD_DIR)/sarahos.iso

clean:
	rm -r $(BUILD_DIR)/*