bits 16
org 0x100

start:
    mov dx,hello_str
    mov ah,9
    int 0x21
    xor ah,ah
    int 0x21

hello_str:
db "Hello World in DOS!",10,'$',0 