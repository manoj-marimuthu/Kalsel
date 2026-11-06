section .data
	minus db '-'
section .bss
	PRINT_BUFFER resb 32

section .text

global print_number

print_number:
	push rbx
	mov rbx, 10	
	cmp rax, 0
	mov rdi, PRINT_BUFFER + 32
	jge convert
	mov r8, rax
	mov rax, 1
	mov rdi, 1
	mov rsi, minus
	mov rdx, 1
	syscall
	mov rax, r8
	neg rax
	mov rdi, PRINT_BUFFER + 32

convert:
	xor rdx, rdx
	div rbx
	add dl, '0'
	dec rdi
	mov byte [rdi], dl
	test rax, rax
	jnz convert

	; prepare for stdout syscall
	mov rsi, rdi
	mov rdx, PRINT_BUFFER + 32
	sub rdx, rsi
	mov rax, 1
	mov rdi, 1
	syscall	
	pop rbx
	ret	
