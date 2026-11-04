section .bss
	VBA resb 80
section .text
global _start
extern print_number
extern print_newline
extern print_space
_start:
	MOV qword [VBA + 0], 0
	MOV RBX, qword [VBA + 0]
	MOV qword [VBA + 8], RBX
	MOV RBX, qword [VBA + 0]
	MOV qword [VBA + 16], RBX
	MOV qword [VBA + 24], 1
	MOV RAX, qword [VBA + 16]
	ADD RAX, qword [VBA + 24]
	MOV qword [VBA + 16], RAX
	MOV RBX, qword [VBA + 0]
	MOV qword [VBA + 32], RBX
	MOV qword [VBA + 40], 2
	MOV RAX, qword [VBA + 32]
	ADD RAX, qword [VBA + 40]
	MOV qword [VBA + 32], RAX
	MOV RAX, qword [VBA + 8]
	call print_number
	call print_space
	MOV RAX, qword [VBA + 16]
	call print_number
	call print_space
	MOV RAX, qword [VBA + 32]
	call print_number
	call print_space
	call print_newline
	MOV qword [VBA + 48], 10
	MOV qword [VBA + 56], 30
	MOV qword [VBA + 64], 30
	 XOR RDX, RDX 
	MOV RAX, qword[VBA + 56]
	DIV qword [VBA + 64]
	MOV qword [VBA + 56], RAX
	MOV RAX, qword [VBA + 48]
	ADD RAX, qword [VBA + 56]
	MOV qword [VBA + 48], RAX
	MOV RAX, qword [VBA + 48]
	call print_number
	call print_space
	call print_newline
exit_program:
MOV RAX, 60
MOV RDI, 0
syscall
