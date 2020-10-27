TARGET = sarahos

BUILD_DIR = build
SRC_DIR = src

TARGET_BIN = $(TARGET).bin
TARGET_ISO = $(TARGET).iso

QEMU_DEBUG_FLAGS = -d int -serial stdio -cdrom $(BUILD_DIR)/$(TARGET_ISO) --no-reboot --no-shutdown

CPP = i686-elf-g++
CPP_FLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib

ASM = nasm
ASM_FLAGS = -felf32

LD = i686-elf-gcc
LD_FLAGS = -ffreestanding -O2 -nostdlib

GRUB_TEST = grub-file --is-x86-multiboot $(BUILD_DIR)/$(TARGET_BIN)

CPP_FILES = $(shell find $(SRC_DIR) -name '*.cpp')
CPP_OBJS = $(patsubst %.cpp,%.o,$(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(CPP_FILES)))

ASM_FILES = $(shell find $(SRC_DIR) -name '*.asm')
ASM_OBJS = $(patsubst %.asm,%.o,$(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(ASM_FILES)))

.PHONY: clean

$(CPP_OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) -c -o $@ $< $(CPP_FLAGS)
	@echo ""

$(ASM_OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASM_FLAGS) -o $@ $<
	@echo ""

kernel: $(CPP_OBJS) $(ASM_OBJS)
	$(LD) -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/$(TARGET_BIN) $(LD_FLAGS) $(ASM_OBJS) $(CPP_OBJS) -lgcc

	@if $(GRUB_TEST); then \
		echo "Multiboot check passed."; \
	else \
		echo "Multiboot check failed!"; \
	fi

iso: kernel $(SRC_DIR)/grub.cfg
	mkdir -p $(BUILD_DIR)/iso/boot/grub

	cp $(BUILD_DIR)/$(TARGET_BIN) $(BUILD_DIR)/iso/boot/$(TARGET_BIN)
	cp $(SRC_DIR)/grub.cfg $(BUILD_DIR)/iso/boot/grub/grub.cfg

	grub-mkrescue -o $(BUILD_DIR)/$(TARGET_ISO) $(BUILD_DIR)/iso

run: iso
	qemu-system-i386.exe -serial stdio -cdrom $(BUILD_DIR)/$(TARGET_ISO)

debug: iso
	qemu-system-i386.exe -serial stdio -d int --no-reboot -cdrom build/sarahos.iso --no-shutdown
	# qemu-system-i386.exe $(QEMU_DEBUG_FLAGS) -cdrom $(BUILD_DIR)/$(TARGET_ISO)

clean:
	rm -r $(BUILD_DIR)/*