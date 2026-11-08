#pragma once

#include<memory>
#include<string>
#include<vector>

enum class AstNodeType{
	OPERATOR,
	NUMBER,
	STRING,
	IDENTIFIER,
	ASSIGNMENT,
	IO_OUT,
	AND,
	OR,
	NOT
};

class AstNode{
	public:
		AstNodeType type;
		std::unique_ptr<AstNode> left{};
		std::unique_ptr<AstNode> right{};
		std::unique_ptr<AstNode> child{};
		std::unique_ptr<AstNode> next{};
		std::string string_data{};
		double num_data{};
		char char_data{};
		AstNode(AstNodeType type):type(type){}
		AstNode(AstNodeType type,std::string string_data):type(type),string_data(string_data){}
		AstNode(AstNodeType type, double num_data):type(type), num_data(num_data){}
		AstNode(AstNodeType type, char char_data):type(type), char_data(char_data){}
	       		
};

extern std::vector<std::unique_ptr<AstNode>> ast_nodes; 
void Parse();
