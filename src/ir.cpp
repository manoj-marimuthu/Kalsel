#include<ir.h>
#include<parser.h>
#include<stdexcept>
#include<string>
#include<vector>
#include<iostream>
#include<memory>
#include<cstdio>
#include<symbol_table.h>
#include<unordered_map>

size_t current_offset = 0;
std::vector<Ir_Instruction> ir_instructions = {};
std::unordered_map<std::string, size_t> symbol_table = {};

size_t generate_ir(const AstNode* node){
	if(!node){
		throw std::runtime_error("generate_ir() received a null ast node \n");
	}
	
	if(node->type == AstNodeType::NUMBER){
		char inst[100];
		snprintf(inst, sizeof(inst), "MOV [VBA + %zu], %d", current_offset * 8, (int) node->num_data);
		Ir_Instruction ir_inst(
					Ir_InstructionType::MOV,
					std::string(inst),
					current_offset,	
					node->num_data
					);
		current_offset += 8;
		ir_instructions.push_back(ir_inst);
		return ir_inst.offset_1;
	}
	else if(node && node->type == AstNodeType::IDENTIFIER){
		char inst[100];
		std::string identifier_name = node->string_data;
		size_t offset = symbol_table[identifier_name];
		snprintf(inst, sizeof(inst), "MOV [VBA + %zu], [VBA + %zu]", current_offset * 8, offset);
		Ir_Instruction ir_inst(
					Ir_InstructionType::MOV_FROM_ST,
					std::string(inst),
					current_offset
				);
		current_offset += 8;
		ir_inst.offset_2 = offset;
		ir_instructions.push_back(ir_inst);
		return ir_inst.offset_1;
	}
	else if(node && node->type == AstNodeType::OPERATOR){
		char inst[100];
		Ir_InstructionType ir_type{};
		size_t left = generate_ir(node->left.get());
		size_t right = generate_ir(node->right.get());
		switch(node->char_data){
			case '+':
				snprintf(inst, sizeof(inst), "ADD [VBA + %zu], [VBA + %zu]", left, right);
				ir_type = Ir_InstructionType::ADD;
				break;

			case '-':
				snprintf(inst, sizeof(inst), "SUB [VBA + %zu], [VBA + %zu]", left, right);
				ir_type = Ir_InstructionType::SUB;
				break;
			case '*':
				snprintf(inst, sizeof(inst), "MUL [VBA + %zu], [VBA + %zu]", left, right);
				ir_type = Ir_InstructionType::MUL;
				break;
			case '/':
				snprintf(inst, sizeof(inst), "DIV [VBA + %zu], [VBA + %zu]", left, right);
				ir_type = Ir_InstructionType::DIV;
				break;
			default:
				throw std::runtime_error("unknown operator found by IR generator \n");
				break;
		}
		Ir_Instruction op(ir_type, inst, left, right);
		ir_instructions.push_back(op);
		return op.offset_1;
	}else if(node && node->type == AstNodeType::ASSIGNMENT){
		std::string variable_name = node->left->string_data;
		if(symbol_table.find(variable_name) != symbol_table.end()){
			size_t original_offset = symbol_table[variable_name];
			size_t new_offset = generate_ir(node->right.get());
			char inst[100];
			snprintf(inst, sizeof(inst), "MOV [VBA + %zu], [VBA + %zu]", original_offset, new_offset);
			Ir_Instruction ir_inst(
					Ir_InstructionType::MOV_FROM_ST,
					inst,
					original_offset
				      );
			ir_inst.offset_2 = new_offset;
			ir_instructions.push_back(ir_inst);
			return new_offset;
		}
		size_t result_offset = generate_ir(node->right.get());
		symbol_table[variable_name] = result_offset;
		return symbol_table[variable_name];
	}
	else if(node && node->type == AstNodeType::IO_OUT){
		std::vector<size_t> locations;
		AstNode* current = node->next.get();
		while(current != nullptr){
			locations.push_back(generate_ir(current));
			current = current->next.get();
		}
		for(const size_t& location: locations){
			char inst[100];
			snprintf(inst, sizeof(inst), "PRINT [VBA + %zu]\n", location);
			Ir_Instruction ir_inst(
						Ir_InstructionType::PRINT,
						inst,
						location
					);
			ir_instructions.push_back(ir_inst);
		}
		return locations[0];
		
	}
	else{
		throw std::runtime_error("Unknown ast type detected by Intermediate Code Generator \n");
	}
	return 0;
}

void traverse_ir_instructions(){
	for(const Ir_Instruction& inst : ir_instructions){
		std::cout << inst.ir_instruction_string << std::endl; 
	}
}
