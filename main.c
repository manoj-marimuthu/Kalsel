#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<gc.h>
#include<token.h>
#include<lexer.h>
#include<parser.h>
#include<tac.h>
#include<asmX86.h>

// for debugging purposes 

int eval_test(Ks_AstNode* exp){
	if(exp->type == AST_NUMBER){
		return exp->data.numData;	
	}else{
		switch(exp->data.charData){
			case '+':
				return eval_test(exp->left) + eval_test(exp->right);	
			case '-':
				return eval_test(exp->left) - eval_test(exp->right);
			case '*':
				return eval_test(exp->left) * eval_test(exp->right);
			case '/':
				return eval_test(exp->left) / eval_test(exp->right);
		}
	}
	return 0;
}

static void raise_compilation_error(){
		Ks_Raise("Failed to compile",-1,COMPILE_TIME_ERROR);
}

static void run_command(const char* command){
	printf("Running : %s \n",command);
	int status = system(command);
	
	if(status != 0){
		raise_compilation_error();
	}
}

int main(){
	FILE* fptr = fopen("test.kal","r");
	if(fptr == NULL){
		Ks_Raise("File not found",-1,FILE_ERROR);
	}
	char line[1024];
	int line_no = 0;
	Ks_WriteStart("asm-modules/program.asm");
	run_command("nasm -f elf64 asm-modules/print_rcx.asm -o build/print_rcx.o");
	while(fgets(line,sizeof(line),fptr) != NULL){
		line_lexer(line,line_no++);
		Ks_Parse(1);
		Ks_GenerateIR(root);
		Ks_GenerateCode("asm-modules/program.asm");
		// run commnds to compile into .asm
		// reset globals that compiler uses
		root = NULL;
		instrs = NULL;
		instr_tail = NULL;
		lexer_output = NULL;
		lexer_tail = NULL;
	}
	Ks_WriteEnd("asm-modules/program.asm");
	run_command("nasm -f elf64 asm-modules/program.asm -o build/program.o");
	run_command("ld build/program.o build/print_rcx.o -o main");
	Ks_Cleanup();
	return 0;
}
