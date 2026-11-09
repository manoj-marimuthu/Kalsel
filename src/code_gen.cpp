#include<code_gen.h>
#include<ir.h>
#include<stdexcept>
#include<fstream>
#include<iostream>

static void attach_header(std::ofstream& file){
	file << "section .bss\n";
	file << "\tVBA resb " << current_offset << "\n";
	file << "section .text\n";
	file << "global _start\n";
	file << "extern print_number\n";
	file << "extern print_newline\n";
	file << "extern print_space\n";
	file << "_start:\n";

}

static void attach_footer(std::ofstream& file){
	file << "exit_program:\n";
	file << "MOV RAX, 60\n";
	file << "MOV RDI, 0\n";
	file << "syscall\n";
}

void code_gen(std::ofstream& file){
	if(!file){
		throw std::runtime_error("Unable to open the given target file \n");
	}
	attach_header(file);
	char inst_str[100];
	for(size_t i = 0;i < ir_instructions.size();i++){
		Ir_Instruction inst = ir_instructions[i];
		if(inst.type == Ir_InstructionType::MOV){
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], %d\n", inst.offset_1, (int) inst.num_data);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::MOV_FROM_ST){
			snprintf(inst_str, sizeof(inst_str), "\tMOV RBX, qword [VBA + %zu]\n", inst.offset_2);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RBX\n", inst.offset_1);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::PRINT){
			while(true){
				snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword [VBA + %zu]\n", inst.offset_1);
				file << inst_str;
				file << "\tcall print_number\n";
				file << "\tcall print_space\n";
				if(i + 1 >= ir_instructions.size() || ir_instructions[i+1].type != Ir_InstructionType::PRINT){
					break;
				}
				inst = ir_instructions[++i];
			}
			file << "\tcall print_newline\n";
		}
		else if(inst.type == Ir_InstructionType::ADD){
			size_t left_offset = inst.offset_1;
			size_t right_offset = inst.offset_2;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword [VBA + %zu]\n", left_offset);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tADD RAX, qword [VBA + %zu]\n", right_offset);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_offset);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::SUB){
			size_t left_offset = inst.offset_1;
			size_t right_offset = inst.offset_2;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword [VBA + %zu]\n", left_offset);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tSUB RAX, qword [VBA + %zu]\n", right_offset);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_offset);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::MUL){
			size_t left_offset = inst.offset_1;
			size_t right_offset = inst.offset_2;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword[VBA + %zu]\n", left_offset);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tIMUL qword [VBA + %zu]\n", right_offset);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_offset);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::DIV){
			size_t left_offset = inst.offset_1;
			size_t right_offset = inst.offset_2;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword[VBA + %zu]\n", left_offset);
			file << inst_str;
			file << "\tCQO\n";
			snprintf(inst_str, sizeof(inst_str), "\tIDIV qword [VBA + %zu]\n", right_offset);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_offset);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::AND){
			size_t left = inst.offset_1;
			size_t right = inst.offset_2;
			// convert lhs to boolean
			file << "\tMOV RAX, [VBA + " << left << "]\n";
		        file << "\tTEST RAX, RAX\n";
			file << "\tSETNE AL\n";
			file << "\tMOVZX RAX, AL\n";
			// convert rhs to boolean
			file << "\tMOV RBX, [VBA + " << right << "]\n";
			file << "\tTEST RBX, RBX\n";
			file << "\tSETNE BL\n";
			file << "\tMOVZX RBX, BL\n";
			// perform bitwise AND between RAX, RBX which may
			// only contain 1 or 0 due to bitwise AND
			file << "\tAND RAX, RBX\n";
			file << "\tMOV [VBA + " << left << "], RAX\n";
		}
		else if(inst.type == Ir_InstructionType::OR){
			size_t left = inst.offset_1;
			size_t right = inst.offset_2;
			// convert lhs to boolean
			file << "\tMOV RAX, [VBA + " << left << "]\n";
		        file << "\tTEST RAX, RAX\n";
			file << "\tSETNE AL\n";
			file << "\tMOVZX RAX, AL\n";
			// convert rhs to boolean
			file << "\tMOV RBX, [VBA + " << right << "]\n";
			file << "\tTEST RBX, RBX\n";
			file << "\tSETNE BL\n";
			file << "\tMOVZX RBX, BL\n";
			// perform bitwise OR between RAX, RBX which may
			// only contain 1 or 0 due to bitwise AND
			file << "\tOR RAX, RBX\n";
			file << "\tMOV [VBA + " << left << "], RAX\n";

		}
		else if(inst.type == Ir_InstructionType::NOT){
			//convert the given input to boolean
			file << "\tMOV RAX, [VBA + " << inst.offset_2 << "]\n";
			file << "\tTEST RAX, RAX\n";
			file << "\tSETE AL\n";
			file << "\tMOVZX RAX, AL\n";
			file << "\tMOV qword [VBA + " << inst.offset_1 << "], RAX\n";
		}
		else if(inst.type == Ir_InstructionType::GT || inst.type == Ir_InstructionType::LT ||
			inst.type == Ir_InstructionType::GEQ || inst.type == Ir_InstructionType::LEQ ||
			inst.type == Ir_InstructionType::EQ || inst.type == Ir_InstructionType::NEQ){
			file << "\tMOV RAX, qword [VBA + " << inst.offset_1 << "]\n";
			file << "\tMOV RBX, qword [VBA + " << inst.offset_2 << "]\n";
			file << "\tCMP RAX, RBX\n";
			std::string operation;
			switch(inst.type){
				case Ir_InstructionType::GT:
					operation = "SETG";
					break;
				case Ir_InstructionType::LT:
					operation = "SETL";
					break;
				case Ir_InstructionType::GEQ:
					operation = "SETGE";
					break;
				case Ir_InstructionType::LEQ:
					operation = "SETLE";
					break;
				case Ir_InstructionType::EQ:
					operation = "SETE";
					break;
				case Ir_InstructionType::NEQ:
					operation = "SETNE";
					break;
				default:
					throw std::runtime_error("Unknown operation found by code generator \n");
					break;
			}
			file << "\t" << operation << " AL\n";
		        file << "\tMOVZX RAX, AL\n";
			file << "\tMOV qword [VBA + " << inst.offset_1 << "], RAX\n";	
		}
		else{
			throw std::runtime_error("Unknown instruction type found during code generation \n");
		}
	}
	attach_footer(file);
	file.close();
}
