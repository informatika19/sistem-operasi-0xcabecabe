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
PY = python3
RM = rm

ksize = 14

CFLAG = -ansi -c

asm_dir = src/asm
c_dir = src/c
out_dir = out
lib_dir = $(c_dir)/lib
prog_dir = $(c_dir)/programs
prog_out_dir = $(out_dir)/programs
lib_obj_dir = $(out_dir)/lib

bootloader_out = $(out_dir)/bootloader
bootloader_asm = $(asm_dir)/bootloader.asm

prog_c = $(wildcard $(prog_dir)/*.c)
prog_c_obj = $(prog_c:$(prog_dir)/%.c=$(prog_out_dir)/%.o)
prog_c_out = $(prog_c_obj:$(prog_out_dir)/%.o=$(prog_out_dir)/%)

lib_c = $(wildcard $(lib_dir)/*.c)
lib_c_obj = $(lib_c:$(lib_dir)/%.c=$(lib_obj_dir)/%.o)
lib_asm = $(asm_dir)/lib.asm
lib_asm_obj = $(lib_obj_dir)/lib_asm.o

kernel_c = $(wildcard $(c_dir)/*.c)
kernel_asm = $(asm_dir)/kernel.asm
kernel_c_obj = $(kernel_c:$(c_dir)/%.c=$(out_dir)/%.o)
kernel_asm_obj = $(out_dir)/kernel_asm.o
kernel = $(out_dir)/kernel

os = $(out_dir)/system.img
map = $(out_dir)/map.img
files = $(out_dir)/files.img
sectors = $(out_dir)/sectors.img

BOCHS_CONFIG = if2230.config

.PHONY: default, img, image, programs, run, clean, out_dir

default: image

$(out_dir):
	mkdir -p $(out_dir) $(prog_out_dir) $(lib_obj_dir)

$(bootloader_out): $(bootloader_asm)
	$(AS) -o $@ $<

# Ini buat bikin object library
$(out_dir)/%.o: $(lib_dir)/%.c
	$(CC) $(CFLAG) -o $@ $<

# Ini buat object kernel
$(out_dir)/%.o: $(c_dir)/%.c
	$(CC) $(CFLAG) -o $@ $<

# Bikin object program
$(prog_out_dir)/%.o: $(prog_dir)/%.c
	$(CC) $(CFLAG) -o $@ $<

# Link program-nya dengan library dan kernel lalu dimasukin ke image os
# Tiap kali program utility ada yg di-update, run `make clean` dlu
$(prog_out_dir)/%: $(prog_out_dir)/%.o $(lib_c_obj) $(lib_asm_obj)
	#$(LD) -o $@ -d $< $(lib_c_obj) $(lib_asm_obj)
	$(LD) -o $@ -d $^
	$(PY) tools/loadFile.py $(os) $@ 0x00

# Compile lib.asm
$(lib_asm_obj): $(lib_asm)
	$(AS) -f as86 -o $@ $<

# Compile kernel.asm
$(kernel_asm_obj): $(kernel_asm)
	$(AS) -f as86 -o $@ $<

# Bikin kernel, link kernel dengan lib dan file asm
$(kernel): $(kernel_c_obj) $(kernel_asm_obj) $(lib_asm_obj) $(lib_c_obj)
	$(LD) -o $@ -d $^

# Bikin image file
$(map):
	$(DD) if=/dev/zero of=$@ bs=512 count=1
	$(PY) -c "import sys; sys.stdout.buffer.write(b'\xFF'*$(ksize))" | $(DD) conv=notrunc bs=$(ksize) count=1 of=$@

$(files):
	$(DD) if=/dev/zero of=$(files) bs=512 count=2
	$(PY) -c "import sys; sys.stdout.buffer.write(b'\xFF\xFF\x62\x69\x6E')" | $(DD) conv=notrunc bs=5 count=1 of=$(files)

$(sectors):
	$(DD) if=/dev/zero of=$(sectors) bs=512 count=1

$(os): $(bootloader_out) $(kernel) $(map) $(files) $(sectors)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(bootloader_out) of=$@ bs=512 count=1 conv=notrunc
	$(DD) if=$(kernel) of=$@ bs=512 conv=notrunc seek=1
	$(DD) if=$(map) of=$@ bs=512 conv=notrunc seek=256
	$(DD) if=$(files) of=$@ bs=512 conv=notrunc seek=257
	$(DD) if=$(sectors) of=$@ bs=512 conv=notrunc seek=259

img: $(out_dir) $(os) programs

programs: $(prog_c_obj) $(prog_c_out)

image: img

run: img
	$(BOCHS) -f $(BOCHS_CONFIG)

clean:
	$(RM) -f out/*.o out/*.img out/lib/*.o out/programs/*.o $(bootloader_out) $(prog_c_out) $(kernel)
