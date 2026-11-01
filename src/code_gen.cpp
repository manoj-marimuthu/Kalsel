#include<code_gen.h>
#include<ir.h>
#include<stdexcept>
#include<fstream>
#include<iostream>

static void attach_header(std::ofstream& file){
	file << "section .bss\n";
	file << "\tVBA resb " << (current_temp_id * 8) + 8 << "\n";
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
		std::runtime_error("Unable to open the given target file \n");
	}
	attach_header(file);
	char inst_str[100];
	for(size_t i = 0;i < ir_instructions.size();i++){
		Ir_Instruction inst = ir_instructions[i];
		if(inst.type == Ir_InstructionType::MOV){
			size_t offset = inst.temp_id;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], %d\n", offset*8, (int) inst.num_data);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::MOV_FROM_ST){
			size_t offset = inst.temp_id;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RBX, qword [VBA + %zu]\n", inst.temp_id_2 * 8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RBX\n", offset * 8);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::PRINT){
			while(true){
				size_t to_print_offset = inst.temp_id;
				snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword [VBA + %zu]\n", to_print_offset*8);
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
			size_t left_temp_id = inst.temp_id;
			size_t right_temp_id = inst.temp_id_2;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword [VBA + %zu]\n", left_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tADD RAX, qword [VBA + %zu]\n", right_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_temp_id*8);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::SUB){
			size_t left_temp_id = inst.temp_id;
			size_t right_temp_id = inst.temp_id_2;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword [VBA + %zu]\n", left_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tSUB RAX, qword [VBA + %zu]\n", right_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_temp_id*8);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::MUL){
			size_t left_temp_id = inst.temp_id;
			size_t right_temp_id = inst.temp_id_2;
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword[VBA + %zu]\n", left_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMUL qword [VBA + %zu]\n", right_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_temp_id*8);
			file << inst_str;
		}
		else if(inst.type == Ir_InstructionType::DIV){
			size_t left_temp_id = inst.temp_id;
			size_t right_temp_id = inst.temp_id_2;
			file << "\t XOR RDX, RDX \n";
			snprintf(inst_str, sizeof(inst_str), "\tMOV RAX, qword[VBA + %zu]\n", left_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tDIV qword [VBA + %zu]\n", right_temp_id*8);
			file << inst_str;
			snprintf(inst_str, sizeof(inst_str), "\tMOV qword [VBA + %zu], RAX\n", left_temp_id*8);
			file << inst_str;
		}
		else{
			throw std::runtime_error("Unknown instruction type found during code generation \n");
		}
	}
	attach_footer(file);
	file.close();
}
