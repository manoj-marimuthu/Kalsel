section .text 
global _start 
extern print_rcx 
extern exit_program 
_start: 
push rbp 
mov rbp,rsp 
sub rsp, 32 
MOV qword [rbp - 8], 1 
MOV qword [rbp - 16], 2 
MOV rax, qword [rbp - 8] 
ADD rax, qword [rbp - 16] 
MOV qword [rbp - 8], rax 
MOV qword [rbp - 24], 3 
MOV rax, qword [rbp - 8] 
ADD rax, qword [rbp - 24] 
MOV qword [rbp - 8], rax 
MOV rcx, [rbp - 8] 
 
 
call print_rcx 
sub rsp, 48 
MOV qword [rbp - 32], 3 
MOV qword [rbp - 40], 4 
MOV rax, qword [rbp - 32] 
SUB rax, qword [rbp - 40] 
MOV qword [rbp - 32], rax 
MOV rcx, [rbp - 32] 
 
 
call print_rcx 
sub rsp, 64 
MOV qword [rbp - 48], 5 
MOV qword [rbp - 56], 30 
MOV rax, qword [rbp - 48] 
SUB rax, qword [rbp - 56] 
MOV qword [rbp - 48], rax 
MOV rcx, [rbp - 48] 
 
 
call print_rcx 
call exit_program 
