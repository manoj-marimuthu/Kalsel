default rel

section .data
	digit db 1
	line_break db 10	
	minus_char db '-'
section .text

global print_rcx
global exit_program

print_rcx:
	mov rax,rcx
	mov r12,10
	mov r13,rsp
	test rax,rax
	js negative
construct:
	cqo
	idiv r12 ; remainder is at  rdx
	push rdx
	cmp rax,0
	jne construct
	jmp deconstruct

negative:
	mov r14,rax ; store rax
	mov rax,1
	mov rdi,1
	lea rsi,[minus_char]
	mov rdx,1
	syscall
	mov rax,r14 ; restore rax
	neg rax
	jmp construct

deconstruct:
	pop rbx
	mov [digit],bl
	add [digit],'0'
	call print_digit
	cmp r13,rsp
	jne deconstruct
	call print_line_break
	ret

print_line_break:
	mov rax,1
	mov rdi,1
	lea rsi,[line_break]
	mov rdx,1
	syscall
	ret

print_digit:
	mov rax,1
	mov rdi,1
	lea rsi,[digit]
	mov rdx,1
	syscall
	ret
	
exit_program:
	mov rax,60
	mov rdi,0
	syscall	
