bits 16
org 0x0

%define int_default_cs  0
%define str_prt_port    0

; In an arbitrary real-mode environment, the IVT is usually stored
; at the base address of 0.
IVT:
; Here is a comprehensive list consisting of 256 Interrupt Handlers.
; Vectors from 0 to 31 are mostly used by Exception Handlers.
dw virt_divide_fault,int_default_cs
dw virt_debug_fault_trap,int_default_cs
dw virt_nmi_handler,int_default_cs
dw virt_breakpoint_trap,int_default_cs
dw virt_overflow_trap,int_default_cs
dw virt_boundrange_fault,int_default_cs
dw virt_invalid_opcode_fault,int_default_cs
dw virt_device_unavail_fault,int_default_cs
dw virt_double_fault_abort,int_default_cs
dw virt_x87_overrun_exception,int_default_cs
dw virt_invalid_tss_fault,int_default_cs
dw virt_inpresent_segment_fault,int_default_cs
dw virt_stack_fault,int_default_cs
dw virt_general_protection_fault,int_default_cs
dw virt_page_fault,int_default_cs
dw virt_reserved_exception,int_default_cs
dw virt_x87_pending_fpu_fault,int_default_cs
dw virt_alignment_check_fault,int_default_cs
dw virt_machine_check_abort,int_default_cs
dw virt_simd_fpu_fault,int_default_cs
dw virt_virtualization_exception,int_default_cs
dw virt_control_protection_fault,int_default_cs
dw virt_reserved_exception,int_default_cs
dw virt_reserved_exception,int_default_cs
dw virt_reserved_exception,int_default_cs
dw virt_reserved_exception,int_default_cs
dw virt_reserved_exception,int_default_cs
dw virt_reserved_exception,int_default_cs
dw virt_hypervisor_injection,int_default_cs
dw virt_vmm_communication,int_default_cs
dw virt_security_exception,int_default_cs
dw virt_reserved_exception,int_default_cs
; Vectors from 32 to 255 are user-defined interrupts.
; Vectors 0x20-0x2F
dw virt_unknown_interrupt,int_default_cs
dw virt_int21_handler,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0x30-0x3F
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0x40-0x4F
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0x50-0x5F
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0x60-0x6F
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0x70-0x7F
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0x80-0x8F
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0x90-0x9F
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0xA0-0xAF
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0xB0-0xBF
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0xC0-0xCF
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0xD0-0xDF
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0xE0-0xEF
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
; Vectors 0xF0-0xFF
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs
dw virt_unknown_interrupt,int_default_cs

virt_int21_handler:
	cmp ah,9
	je int21_print_string_stdout
	cmp ah,0
	je int21_termination
	iret

int21_print_string_stdout:
	mov si,dx
	call strlen
	mov cx,dx
	mov dx,str_prt_port
	rep outsb
	iret

int21_termination:
	call print_halted
	cli
	hlt

virt_divide_fault:
virt_debug_fault_trap:
virt_nmi_handler:
virt_breakpoint_trap:
virt_overflow_trap:
virt_boundrange_fault:
virt_invalid_opcode_fault:
virt_device_unavail_fault:
virt_double_fault_abort:
virt_x87_overrun_exception:
virt_invalid_tss_fault:
virt_inpresent_segment_fault:
virt_stack_fault:
virt_general_protection_fault:
virt_page_fault:
virt_x87_pending_fpu_fault:
virt_alignment_check_fault:
virt_machine_check_abort:
virt_simd_fpu_fault:
virt_virtualization_exception:
virt_control_protection_fault:
virt_hypervisor_injection:
virt_vmm_communication:
virt_security_exception:
virt_reserved_exception:
	call print_exception

virt_unknown_interrupt:
	call print_interrupt

print_exception:
	mov si,exception_print_string
	mov cx,100
	call strlen
	mov cx,dx
	mov dx,str_prt_port
	mov si,exception_print_string
	mov ds,dx
	rep outsb
	cli
	hlt

print_interrupt:
	mov si,interrupt_print_string
	mov cx,100
	call strlen
	mov cx,dx
	mov dx,str_prt_port
	mov si,interrupt_print_string
	mov ds,dx
	rep outsb
	cli
	hlt

print_halted:
	mov si,halted_print_string
	mov cx,100
	call strlen
	mov cx,dx
	mov dx,str_prt_port
	mov si,halted_print_string
	mov ds,dx
	; rep outsb
	cli
	hlt

exception_print_string:
db "[Firmware] Exception Encountered!",10,'$',0
interrupt_print_string:
db "[Firmware] Unknown Interrupt!",10,'$',0
halted_print_string:
db "[Firmware] Halted due to int21h! /w ah=0",10,'$',0

strlen:
	cld
	xor dx,dx
@1:
	lodsb
	cmp al,'$'
	jz @2
	inc dx
	loop @1
@2:
	sub si,dx
	dec si
	ret