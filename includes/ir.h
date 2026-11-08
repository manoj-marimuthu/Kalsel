#ifndef IR_H
#define IR_H

#include<string>
#include<memory>
#include<vector>
#include<parser.h>

enum class Ir_InstructionType{
	MOV,
	MOV_FROM_ST,
	ADD,
	MUL,
	SUB,
	DIV,
	AND,
	OR,
	NOT,
	PRINT,
	PRINT_CON,
	NULL_NODE
};

class Ir_Instruction{
	public:
		Ir_InstructionType type;
		std::string ir_instruction_string{};
		size_t offset_1{};
		size_t offset_2{};
		double num_data{};
		Ir_Instruction(Ir_InstructionType type):type(type){}
		Ir_Instruction(Ir_InstructionType type, std::string ir_instruction_string, size_t offset_1):
			type(type), ir_instruction_string(ir_instruction_string), offset_1(offset_1){}
		Ir_Instruction(Ir_InstructionType type, std::string ir_instruction_string, size_t offset_1, size_t offset_2):
			type(type), ir_instruction_string(ir_instruction_string), offset_1(offset_1), offset_2(offset_2){}
		Ir_Instruction(Ir_InstructionType type, std::string ir_instruction_string, size_t offset_1
				,double num):
			type(type), ir_instruction_string(ir_instruction_string), offset_1(offset_1), num_data(num){}

};

extern size_t current_offset;
extern std::vector<Ir_Instruction> ir_instructions;
size_t generate_ir(const AstNode* node);
void traverse_ir_instructions();
#endif
