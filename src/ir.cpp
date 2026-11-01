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

size_t current_temp_id = 0;
std::vector<Ir_Instruction> ir_instructions = {};
std::unordered_map<std::string, size_t> symbol_table = {};

size_t generate_ir(const AstNode* node){
	if(!node){
		throw std::runtime_error("generate_ir() received a null ast node \n");
	}
	
	if(node->type == AstNodeType::NUMBER){
		char inst[100];
		snprintf(inst, sizeof(inst), "MOV t%zu, %d", current_temp_id, (int) node->num_data);
		Ir_Instruction ir_inst(
					Ir_InstructionType::MOV,
					std::string(inst),
					current_temp_id++,	
					node->num_data
					);
		ir_instructions.push_back(ir_inst);
		return ir_inst.temp_id;
	}
	else if(node && node->type == AstNodeType::IDENTIFIER){
		char inst[100];
		std::string identifier_name = node->string_data;
		size_t offset = symbol_table[identifier_name];
		snprintf(inst, sizeof(inst), "MOV t%zu, [VBA + %zu]", current_temp_id, 8 * offset);
		Ir_Instruction ir_inst(
					Ir_InstructionType::MOV_FROM_ST,
					std::string(inst),
					current_temp_id++
				);
		ir_inst.temp_id_2 = 8 * offset;
		ir_instructions.push_back(ir_inst);
		return ir_inst.temp_id;
	}
	else if(node && node->type == AstNodeType::OPERATOR){
		char inst[100];
		Ir_InstructionType ir_type{};
		size_t left = generate_ir(node->left.get());
		size_t right = generate_ir(node->right.get());
		switch(node->char_data){
			case '+':
				snprintf(inst, sizeof(inst), "ADD t%zu, t%zu", left, right);
				ir_type = Ir_InstructionType::ADD;
				break;

			case '-':
				snprintf(inst, sizeof(inst), "SUB t%zu, t%zu", left, right);
				ir_type = Ir_InstructionType::SUB;
				break;
			case '*':
				snprintf(inst, sizeof(inst), "MUL t%zu, t%zu", left, right);
				ir_type = Ir_InstructionType::MUL;
				break;
			case '/':
				snprintf(inst, sizeof(inst), "DIV t%zu, t%zu", left, right);
				ir_type = Ir_InstructionType::DIV;
				break;
			default:
				throw std::runtime_error("unknown operator found by IR generator \n");
				break;
		}
		Ir_Instruction op(ir_type, inst, left, right);
		ir_instructions.push_back(op);
		return op.temp_id;
	}else if(node && node->type == AstNodeType::ASSIGNMENT){
		std::string variable_name = node->left->string_data;
		size_t result_temp_id = generate_ir(node->right.get());
		symbol_table[variable_name] = result_temp_id;
		return result_temp_id;
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
			snprintf(inst, sizeof(inst), "PRINT [VBA + %zu]\n", location * 8);
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
