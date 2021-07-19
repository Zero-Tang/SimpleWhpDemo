bits 16
org 0x100

segment .text
start:
    mov dx,hello_str
    mov ah,9
    int 0x21
    xor ah,ah
    int 0x21

segment .data
hello_str:
db "Hello Real-Mode Programming!",10,'$',0 