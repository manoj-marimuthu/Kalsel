#include <tac.h>
#include <gc.h>
#include <error.h>
#include <stdlib.h>
#include <parser.h>
#include<stdio.h>

Ks_Instruction* instrs = NULL;
Ks_Instruction* instr_tail = NULL;
unsigned int temp_id = 1;

static int getTempId(){
	return temp_id++;
}

Ks_Instruction* Ks_CreateInstruction(){
	Ks_Instruction* inst = Ks_Allocate(sizeof(Ks_Instruction));
	return inst;
}

void Ks_EmitInstruction(Ks_Instruction* inst){
	if(instrs == NULL){
		instrs = inst;
		instr_tail = inst;
	}else{
		instr_tail->next = inst;
		instr_tail = inst;
	}
}


int Ks_GenerateIR(Ks_AstNode* node){
	Ks_Instruction* inst = Ks_CreateInstruction();
	if(node->type == AST_NUMBER){
		inst->lhs_temp_id = 0;
		inst->rhs_temp_id = 0;
		inst->result_temp_id = getTempId();
		inst->type = MOV_INST;
		inst->numData = node->data.numData;
		inst->op = ' ';
	}else{
		inst->type = OP_INST;
		inst->op = node->data.charData;
		inst->lhs_temp_id = Ks_GenerateIR(node->left);
		inst->rhs_temp_id = Ks_GenerateIR(node->right);	
		inst->result_temp_id = inst->lhs_temp_id;
		inst->numData = 0;
	}
	Ks_EmitInstruction(inst);
	return inst->result_temp_id;
}

void traverse_ir(){
	Ks_Instruction* curInstr = instrs;
	while(curInstr){
		if(curInstr->type == MOV_INST){
			printf("MOV t%d, %d",curInstr->result_temp_id,curInstr->numData);
		}else{
			switch(curInstr->op){
				case '+':
					printf("ADD t%d,t%d",curInstr->lhs_temp_id,curInstr->rhs_temp_id);
					break;	
				case '-':
					printf("SUB t%d,t%d",curInstr->lhs_temp_id,curInstr->rhs_temp_id);
					break;
				case '*':
					printf("MUL t%d,t%d",curInstr->lhs_temp_id,curInstr->rhs_temp_id);
					break;
				case '/':
					printf("DIV t%d,t%d",curInstr->lhs_temp_id,curInstr->rhs_temp_id);
					break;
				default:
					break;
			}
		}
		printf("\n");
		curInstr = curInstr->next;
	}
	curInstr = instrs;
	while(curInstr){
		printf("Instruction data: \n");
		printf("LHS_TEMP_ID = %d \n",curInstr->lhs_temp_id);
		printf("RHS_TEMP_ID = %d \n",curInstr->rhs_temp_id);
		printf("RESULT_TEMP_ID = %d \n",curInstr->result_temp_id);
		curInstr = curInstr->next;
	}
}

