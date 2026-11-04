#include<iostream>
#include<lexer.h>
#include<parser.h>
#include<ir.h>
#include<symbol_table.h>
#include<code_gen.h>
#include<vector>
#include<string>
#include<fstream>
#include<cstdlib>

#define VERSION "v0.0.0"
#define TARGET "x86_64 Linux-GNU"

std::vector<Token> tokens{};
size_t current_token_index = 0;

void reset_states(){
	ir_instructions.clear();
	tokens.clear();
	current_token_index = 0;
}

static void COMPILE(std::ifstream& file,std::string& output_file_name, bool show_ir){
	reset_states();
	std::string line;
	size_t line_no = 0;
	while(std::getline(file,line)){
		line_lexer(line, line_no++);
	}
	tokens.push_back(Token(TokenType::END_OF_FILE));
	Parse();
	for(const auto& ast_node: ast_nodes){
		generate_ir(ast_node.get());
	}
	if(show_ir){
		std::cout << "KALSEL IR:" << std::endl;
		traverse_ir_instructions();
		std::cout << "IR END" << std::endl;
	}
	std::ofstream out_file("asm-modules/program.asm",std::ios::out | std::ios::trunc);
	code_gen(out_file);
	std::cout << "COMPILATION to x86 asm suceeded" << std::endl;
	int result = std::system("nasm -f elf64 asm-modules/program.asm -o build/program.o");
	if(result != 0) throw std::runtime_error("KALSEL failed to produce object files for target file \n");
	result = std::system("nasm -f elf64 asm-modules/print.asm -o build/print.o");
	if(result != 0) throw std::runtime_error("KALSEL failed to produce object files for target file \n");
	result = std::system("nasm -f elf64 asm-modules/print_newline.asm -o build/print_newline.o");
	if(result != 0) throw std::runtime_error("KALSEL failed to produce object files for target file \n");
	char cmd[100];
	if(output_file_name == "") output_file_name = "main";
	snprintf(cmd, sizeof(cmd), "ld build/program.o build/print.o build/print_newline.o -o %s", output_file_name.c_str());
	result = std::system(cmd);
	if(result != 0) throw std::runtime_error("KALSEL failed to produce object files for target file \n");
	std::cout << "COMPILATION to object files and linking succeeded" << std::endl;
}

int main(int argc, char* argv[]){
	bool show_ir = false;
	std::string file_name{};
	std::string output_file_name{};
	if(argc < 2){
		throw std::runtime_error("No flags or target file names provided \n`");
	}
	bool only_meta = true;
	for(int i = 1;i < argc;i++){
		std::string arg = argv[i];
		if(arg == "--info" || arg == "-i"){
			std::cout << "Kalsel Compiler" << std::endl;
			std::cout << "VERSION : V0.0.0" << std::endl;
			std::cout << "OUTPUT : NASM assembly" << std::endl;
			std::cout << "TARGET : x86_64 Linux-GNU" << std::endl;
			return 0;
		}else if(arg == "--target" || arg == "-t"){
			std::cout << TARGET << std::endl;
		}
		else if(arg == "--version" || arg == "-v"){
			std::cout << VERSION << std::endl;
		}
		else if(arg == "--ir"){
			show_ir = true;	
		}else if(arg.size() > 4 && arg.substr(arg.size() - 4) == ".kal"){
			file_name = arg;
			only_meta = false;
		}
		else if(arg == "-o"){
			if(i + 1 < argc){
				output_file_name = argv[++i];
				only_meta = false;
			}else{
				throw std::runtime_error("Compiler needs target file name after '-o' \n");
			}
		}
		else{
			char msg[50];
			snprintf(msg, sizeof(msg), "Unknown flag '%s' given to the compiler \n", arg.c_str());
			throw std::runtime_error(std::string(msg));
		}

	}
	if(only_meta){
		if(show_ir){
			throw std::runtime_error("cannot use --ir, since no target file is given \n");
		}
		return 0;
	}
	std::ifstream file(file_name);
	if(!file.is_open()){
		throw std::runtime_error("Could' nt open file for compilation \n");
	}
	COMPILE(file, output_file_name, show_ir);
	return 0;
}
