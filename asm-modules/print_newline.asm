section .data
	space db ' '	
	newline db 10
section .text

global print_newline
global print_space

print_newline:
	mov rax, 1
	mov rdi, 1
	mov rsi, newline
	mov rdx, 1
	syscall
	ret

print_space:
	mov rax, 1
	mov rdi, 1
	mov rsi, space
	mov rdx, 1
	syscall
	ret
