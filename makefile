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
LIB_DIR = $(C_DIR)/lib

BOOTLOADER_OUT = $(OUT_DIR)/bootloader
BOOTLOADER_ASM = $(ASM_DIR)/bootloader.asm

LIB_C = $(wildcard $(LIB_DIR)/*.c)
LIB_C_OUT = $(patsubst $(LIB_DIR)/%.c, $(OUT_DIR)/%.o, $(LIB_C))
LIB_ASM = $(ASM_DIR)/lib.asm
LIB_ASM_OUT = $(OUT_DIR)/lib_asm.o

KERNEL_C = $(C_DIR)/kernel.c
KERNEL_ASM = $(ASM_DIR)/kernel.asm
KERNEL_C_OUT = $(OUT_DIR)/kernel.o
KERNEL_ASM_OUT = $(OUT_DIR)/kernel_asm.o
KERNEL = $(OUT_DIR)/kernel

IMG = $(OUT_DIR)/system.img

BOCHS_CONFIG = if2230.config

default: image

$(OUT_DIR):
	mkdir $@

$(BOOTLOADER_OUT): $(BOOTLOADER_ASM)
	$(AS) -o $@ $<

$(OUT_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) -ansi -c -o $@ $<

$(LIB_ASM_OUT): $(LIB_ASM)
	$(AS) -f as86 -o $@ $<

$(KERNEL_C_OUT): $(KERNEL_C)
	$(CC) -ansi -c -o $@ $<

$(KERNEL_ASM_OUT): $(KERNEL_ASM)
	$(AS) -f as86 -o $@ $<

$(KERNEL): $(KERNEL_C_OUT) $(LIB_C_OUT) $(KERNEL_ASM_OUT) $(LIB_ASM_OUT)
	# Urutan linker ternyata ngaruh :O
	$(LD) -o $@ -d $^

$(IMG): $(OUT_DIR) $(BOOTLOADER_OUT) $(KERNEL)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(BOOTLOADER_OUT) of=$@ bs=512 count=1 conv=notrunc
	$(DD) if=$(KERNEL) of=$@ bs=512 conv=notrunc seek=1

image: $(IMG)

run: $(IMG)
	$(BOCHS) -f $(BOCHS_CONFIG)

clean:
	rm -rf out/*
