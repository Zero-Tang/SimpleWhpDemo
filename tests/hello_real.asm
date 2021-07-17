bits 16
org 0x100

; Define special I/O Ports for hypercall.
%define str_prt_port	0

segment .text
start:
	; Get the length of string.
	mov si,hello_str
	mov cx,100
	call strlen
	; Perform Output
	mov cx,dx
	mov dx,str_prt_port
	mov si,hello_str
	rep outsb
	; Terminate this program.
	cli
	hlt

; Input Parameters:
; ds:si points to the string.
; cx indicates the maximum number of characters.
; Return Value:
; dx indicates the number of characters.
; This function automatically clears the DF flag.
strlen:
	cld
	xor dx,dx
@1:
	lodsb
	test al,al
	inc dx
	jz @2
	loop @1
@2:
	ret

segment .data
hello_str:
db "Hello Real-Mode Programming!",10,0