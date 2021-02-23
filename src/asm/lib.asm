; lib.asm
; Alvin W., Josep M., Rehagana K.C.S.
; 20 Februari 2020

; lib.asm berisi fungsi-fungsi bantuan kernel yang tidak bisa didefinisikan di
; file lib.c

; global _loadBin
global _getCursorRow

# int *loadBinary()
; _loadBin:
; 	pusha
; 	push bp
; 	mov bp,sp
; 	mov si,.binFile
; 	mov ax,si
; 	pop bp
; 	popa

; 	ret

; 	.binFile: incbin "out/image.bin"

;int getCursor()
_getCursorRow:
	push bp
	mov bp,sp
	mov ah,0x03
	xor bh,bh
	int 0x10

	mov al,dh	;ambil row nya
	mov ah,0	;return row yg ditaro di AL
	pop bp
	ret