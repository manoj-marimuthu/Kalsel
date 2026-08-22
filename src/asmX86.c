#include<asmX86.h>
#include<stdlib.h>
#include<stdio.h>
#include<parser.h>
#include<tac.h>
#include<error.h>
#include<string.h>

void Ks_WriteStart(char* fileName){	
	FILE* fptr = fopen(fileName,"w");
	if(fptr == NULL){
		Ks_Raise("File not found for compilation",-1,FILE_ERROR);
	}
	fprintf(fptr,"section .text \n");
	fprintf(fptr,"global _start \n");
	fprintf(fptr,"extern print_rcx \n");
	fprintf(fptr,"extern exit_program \n");
	fprintf(fptr,"_start: \n");
	fprintf(fptr,"\tpush rbp \n");
	fprintf(fptr,"\tmov rbp,rsp \n");
	fclose(fptr);
}

void Ks_WriteEnd(char* fileName){
	FILE* fptr = fopen(fileName,"a");
	if(fptr == NULL){
		Ks_Raise("File not found for compilation",-1,FILE_ERROR);
	}
	fprintf(fptr, "\tcall exit_program \n");
	fclose(fptr);	
}

static void call_print_rcx(FILE* fptr){
	fprintf(fptr,"\tcall print_rcx \n");
}

void Ks_GenerateCode(char* fileName){
	Ks_Instruction* inst = instrs;
	FILE* fptr = fopen(fileName,"a");
	if(fptr == NULL){
		Ks_Raise("File not found for compilation",-1,FILE_ERROR);
	}
	char asmCode[124];
	int offset;
	Ks_Instruction* last_instr= NULL;	
	int needed_space = temp_id * 8;
	fprintf(fptr,"\tsub rsp, %d \n",needed_space);

	while(inst){
		if(inst->type == MOV_INST){
			offset = inst->result_temp_id * 8; 
			snprintf(asmCode, sizeof(asmCode), "\tMOV qword [rbp - %d], %d \n",offset,inst->numData);			
		}else{
			switch(inst->op){
				case '+':
					snprintf(asmCode,sizeof(asmCode), 
					        "\tMOV rax, qword [rbp - %d] \n"
						"\tADD rax, qword [rbp - %d] \n"
						"\tMOV qword [rbp - %d], rax \n"
						,
						inst->lhs_temp_id * 8,inst->rhs_temp_id * 8,inst->lhs_temp_id * 8
					);
					break;
				case '-':
					snprintf(asmCode,sizeof(asmCode), 
						"\tMOV rax, qword [rbp - %d] \n"
						 "\tSUB rax, qword [rbp - %d] \n"
						 "\tMOV qword [rbp - %d], rax \n"
						,
						inst->lhs_temp_id * 8,inst->rhs_temp_id * 8,inst->lhs_temp_id * 8
					);
					break;
				case '*':
					snprintf(asmCode, sizeof(asmCode),
					"\tMOV rax, qword [rbp - %d]\n"
					"\tIMUL rax,qword [rbp - %d]\n"
					"\tMOV qword [rbp - %d],rax\n"
					,
					inst->lhs_temp_id * 8, inst->rhs_temp_id * 8, inst->lhs_temp_id * 8);
					break;
				case '/':
					snprintf(asmCode, sizeof(asmCode),
					"\tMOV rax, qword [rbp - %d]\n"
					"\tMOV rbx, qword [rbp - %d]\n"
					"\tcqo\n"
					"\tIDIV rbx\n"
					"\tMOV qword [rbp - %d],rax \n"
					,
					inst->lhs_temp_id * 8, inst->rhs_temp_id * 8,inst->lhs_temp_id * 8);
					break;
				default:
					break;
			}
		}
		fprintf(fptr,"%s",asmCode);
		last_instr = inst;
		inst = inst->next;
	}
	snprintf(asmCode, sizeof(asmCode), "\tMOV rcx, [rbp - %d] \n",last_instr->result_temp_id * 8);
	fprintf(fptr, "%s \n \n", asmCode);
	call_print_rcx(fptr);
	fclose(fptr);
}
