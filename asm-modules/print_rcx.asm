default rel

section .bss
	temp resb 256

%define INITIAL_STACK_POINTER temp
%define DIV_DENOMINATOR temp + 8
%define RAX_TEMP temp + 16
%define CURRENT_DIGIT temp + 24

section .data
	line_break db 10
	minus db '-'

section .text

extern print_rcx
extern exit_program

print_rcx:
	mov qword [INITIAL_STACK_POINTER],rsp	
	mov qword [DIV_DENOMINATOR], 10
	mov rax,rcx
	test rax,rax
	js negative

construct:
	cqo
	idiv qword [DIV_DENOMINATOR] ; remainder is at rdx
	push rdx
	cmp rax,0
	jne construct
	jmp deconstruct

negative:
	mov qword [RAX_TEMP], rax
	mov rax,1
	mov rdi,1
	lea rsi,[minus]
	mov rdx,1
	syscall
	mov rax, qword [RAX_TEMP]
	neg rax
	jmp construct

deconstruct:
	pop rbx
	call print_digit
	cmp rsp, qword [INITIAL_STACK_POINTER]
	jne deconstruct
	call print_line_break
	ret

print_digit:
	mov qword [CURRENT_DIGIT], rbx
	mov rax,1
	mov rdi,1
	add qword [CURRENT_DIGIT], '0'
	lea rsi, qword [CURRENT_DIGIT]
	mov rdx,1
	syscall
	ret

print_line_break:
	mov rax,1
	mov rdi,1
	lea rsi,[line_break]
	mov rdx,1
	syscall
	ret

exit_program:
	mov rax,60
	mov rdx,0
	syscall




