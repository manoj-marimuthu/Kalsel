#ifndef TAC_H
#define TAC_H

#include<parser.h>

typedef enum {
	OP_INST,
	MOV_INST
} Ks_InstructionType;


typedef struct Ks_Instruction{
	Ks_InstructionType type;
	int result_temp_id;
	int lhs_temp_id;
	int rhs_temp_id;
	char op;	
	int numData;
	struct Ks_Instruction* next;
} Ks_Instruction;

extern Ks_Instruction* instrs;
extern Ks_Instruction* instr_tail;
extern unsigned int temp_id;

Ks_Instruction* Ks_CreateInstruction();
void Ks_EmitInstruction(Ks_Instruction* inst);
int Ks_GenerateIR(Ks_AstNode* node);
void traverse_ir();
#endif
