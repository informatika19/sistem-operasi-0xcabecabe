; lib.asm
; Alvin W., Josep M., Rehagana C.K.
; 20 Februari 2020

; lib.asm berisi fungsi-fungsi bantuan kernel yang tidak bisa didefinisikan di
; lib.c

global _getCursor

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
