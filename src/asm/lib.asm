; lib.asm
; Alvin W., Josep M., Rehagana K.C.S.
; 20 Februari 2021

; lib.asm berisi fungsi-fungsi bantuan kernel yang tidak bisa didefinisikan di
; file lib.c

global _getCursorRow
global _getCursorCol
global _interrupt
global _putInMemory
global _launchProgram

;int getCursorCol()
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

;int getCursorCol()
_getCursorCol:
	push bp
	mov bp,sp
	mov ah,0x03
	xor bh,bh
	int 0x10

	mov al,dl	;ambil col nya // al = dl
	mov ah,0	;return col yg ditaro di AL
	pop bp
	ret

;int interrupt (int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]	;get the interrupt number in AL
	push ds		;use self-modifying code to call the right interrupt
	mov bx,cs
	mov ds,bx
	mov si,intr
	mov [si+1],al	;change the 00 below to the contents of AL
	pop ds
	mov ax,[bp+6]	;get the other parameters AX, BX, CX, and DX
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	int 0x00	;call the interrupt (00 will be changed above)

	mov ah,0	;we only want AL returned
	pop bp
	ret

; menjalankan sebuah program
; void launchProgram(int segment)
_launchProgram:
    mov bp,sp
    mov bx,[bp+2]

    mov ax,cs
    mov ds,ax
    mov si,jump
    mov [si+3],bx

    mov ds,bx
    mov ss,bx
    mov es,bx

    mov sp,0xfff0
    mov bp,0xfff0

jump:    jmp 0x0000:0x0000

;void putInMemory (int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

