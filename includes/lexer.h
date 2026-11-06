#pragma once

#include<string>
#include<vector>

enum class TokenType{
	NUMBER,
	IDENTIFIER,
	SEMI_COLON,
	EQUAL,
	INT,
	OPERATOR,
	IO,
	L_BRACK,
	R_BRACK,
	DOUBLE_LT,
	END_OF_FILE
};

class Token{
	public:
		TokenType type;
		std::string string_data{};
		double num_data{};
		char char_data{};
		Token(){}
		Token(TokenType type):type(type){}
		Token(TokenType type, std::string string_data):type(type),string_data(string_data){}
		Token(TokenType type, double num_data):type(type),num_data(num_data){}
		Token(TokenType type, char char_data):type(type),char_data(char_data){}
};

extern std::vector<Token> tokens;
extern size_t current_token_index;
void line_lexer(const std::string& line, size_t line_no);
void traverse_tokens(const std::vector<Token>& tokens);
