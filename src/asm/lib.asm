; lib.asm
; Alvin W., Josep M., Rehagana K.C.S.
; 20 Februari 2020

; lib.asm berisi fungsi-fungsi bantuan kernel yang tidak bisa didefinisikan di
; file lib.c

global _getCursor
global _printLogo
global _loadBin

_printLogo:
	; si = image source
	; mov si, imageFile
	pusha
	xor ax, ax
	lodsb
	mov cx, ax ;x-position
	lodsb
	mov dx, ax ;y-position
	.for_x:
		push dx
		.for_y:
			mov bh, 0  ;page number
			lodsb       ;al (color) -> next byte
			mov ah, 0xC ;write pixel at coordinate
			int 0x10 ;might "destroy" ax, si and di on some systems
		sub dx, 1  ;decrease dx by one and set flags
		jnz .for_y ;repeat for y-length
		pop dx     ;restore dx
	sub cx, 1      ;decrease si by one and set flags
	jnz .for_x     ;repeat for x-length
	popa
	ret

# int *loadBinary()
_loadBin:
	pusha
	push bp
	mov bp,sp
	mov si,.binFile
	mov ax,si
	pop bp
	popa

	ret

	.binFile: incbin "out/image.bin"

;int getCursor()
_getCursor:
	push bp
	mov bp,sp
	mov ah,0x03
	xor bh,bh
	int 0x10

	mov al,dh	;ambil row nya
	mov ah,0	;return row yg ditaro di AL
	pop bp
	ret

imageFile: incbin "out/image.bin" ;include the image binary