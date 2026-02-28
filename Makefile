# RaisuOS Makefile
# x86_64 kernel build system

CC = x86_64-elf-gcc
AS = nasm
LD = x86_64-elf-ld

KERNEL_DIR = kernel
BUILD_DIR = build
ISO_DIR = iso

CFLAGS = -std=c11 -ffreestanding -O2 -Wall -Wextra \
         -I$(KERNEL_DIR)/include -fno-stack-protector \
         -nostdlib -fno-builtin -fno-pie \
         -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -c

ASFLAGS = -f elf64
LDFLAGS = -n -T $(KERNEL_DIR)/arch/x86_64/linker.ld -nostdlib -z max-page-size=0x1000

ASM_SRCS = $(KERNEL_DIR)/arch/x86_64/boot.asm \
           $(KERNEL_DIR)/arch/x86_64/isr.asm

C_SRCS = $(KERNEL_DIR)/kernel.c \
         $(KERNEL_DIR)/arch/x86_64/gdt.c \
         $(KERNEL_DIR)/arch/x86_64/idt.c \
         $(KERNEL_DIR)/lib/string.c \
         $(KERNEL_DIR)/lib/math.c \
         $(KERNEL_DIR)/mm/heap.c \
         $(KERNEL_DIR)/drivers/timer.c \
         $(KERNEL_DIR)/drivers/serial.c \
         $(KERNEL_DIR)/drivers/keyboard.c \
         $(KERNEL_DIR)/drivers/mouse.c \
         $(KERNEL_DIR)/drivers/vbe.c \
         $(KERNEL_DIR)/gui/graphics.c \
         $(KERNEL_DIR)/gui/font.c \
         $(KERNEL_DIR)/gui/window.c \
         $(KERNEL_DIR)/gui/desktop.c \
         $(KERNEL_DIR)/gui/taskbar.c \
         $(KERNEL_DIR)/gui/launcher.c \
         $(KERNEL_DIR)/apps/app_terminal.c \
         $(KERNEL_DIR)/apps/app_editor.c \
         $(KERNEL_DIR)/apps/app_files.c \
         $(KERNEL_DIR)/apps/app_sysmon.c \
         $(KERNEL_DIR)/apps/app_settings.c \
         $(KERNEL_DIR)/apps/app_about.c \
         $(KERNEL_DIR)/i18n/i18n.c

ASM_OBJS = $(patsubst $(KERNEL_DIR)/arch/x86_64/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))
C_OBJS = $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))

KERNEL_BIN = $(BUILD_DIR)/raisuos.bin
ISO_FILE = $(BUILD_DIR)/raisuos.iso

.PHONY: all clean iso

all: $(KERNEL_BIN)

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/arch/x86_64/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(KERNEL_BIN): $(ASM_OBJS) $(C_OBJS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^

iso: $(KERNEL_BIN)
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/raisuos.bin
	cp $(KERNEL_DIR)/arch/x86_64/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ISO_DIR)/boot/raisuos.bin
