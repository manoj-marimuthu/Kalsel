section .bss
	VBA resb 40
section .text
global _start
extern print_number
extern print_newline
extern print_space
_start:
	MOV qword [VBA + 0], 10
	MOV RBX, qword [VBA + 0]
	MOV qword [VBA + 8], RBX
	MOV RAX, qword [VBA + 8]
	call print_number
	call print_space
	call print_newline
	MOV qword [VBA + 16], 20
	MOV RBX, qword [VBA + 16]
	MOV qword [VBA + 0], RBX
	MOV RBX, qword [VBA + 0]
	MOV qword [VBA + 24], RBX
	MOV RAX, qword [VBA + 24]
	call print_number
	call print_space
	call print_newline
exit_program:
MOV RAX, 60
MOV RDI, 0
syscall
