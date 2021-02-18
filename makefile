#all: library.cpp main.cpp

#In this case:

    #$@ evaluates to all
    #$< evaluates to library.cpp
    #$^ evaluates to library.cpp main.cpp

CC = bcc
DD = dd
LD = ld86
BOCHS = bochs
AS = nasm

ASM_DIR = src/asm
C_DIR = src/c
OUT_DIR = out

BOOTLOADER_OUT = $(OUT_DIR)/bootloader
BOOTLOADER_ASM = $(ASM_DIR)/bootloader.asm

KERNEL_C = $(C_DIR)/kernel.c
KERNEL_ASM = $(ASM_DIR)/kernel.asm
KERNEL_C_OUT = $(OUT_DIR)/kernel.o
KERNEL_ASM_OUT = $(OUT_DIR)/kernel_asm.o
KERNEL = $(OUT_DIR)/kernel

IMG = $(OUT_DIR)/system.img

BOCHS_CONFIG = if2230.config

default: clean $(IMG)

$(OUT_DIR):
	mkdir $@

$(BOOTLOADER_OUT): $(BOOTLOADER_ASM) $(OUT_DIR)
	$(AS) -o $@ $<

$(KERNEL_C_OUT): $(KERNEL_C) $(OUT_DIR)
	$(CC) -ansi -c -o $@ $<

$(KERNEL_ASM_OUT): $(KERNEL_ASM) $(OUT_DIR)
	$(AS) -f as86 -o $@ $<

$(KERNEL): $(KERNEL_C_OUT) $(KERNEL_ASM_OUT) # Urutan linker ternyata ngaruh :O
	$(LD) -o $@ -d $^

$(IMG): $(BOOTLOADER_OUT) $(KERNEL)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(BOOTLOADER_OUT) of=$@ bs=512 count=1 conv=notrunc
	$(DD) if=$(KERNEL) of=$@ bs=512 conv=notrunc seek=1

run: $(IMG)
	$(BOCHS) -f $(BOCHS_CONFIG)

clean:
	rm -rf out/*
