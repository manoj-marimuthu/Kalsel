#include<iostream>
#include<string>
#include<vector>
#include<stdexcept>
#include<cctype>
#include<lexer.h>

void line_lexer(const std::string& line, size_t line_no){
	int i = 0;
	int n = line.size();
	while(i < n){
		if(std::isspace(line[i])){
			i++;
			continue;
		}
		else if(std::isalpha(line[i]) || line[i] == '_'){
			std::string identifier_name = "";
			while(i < n && (std::isalnum(line[i]) || line[i] == '_')){
				identifier_name += line[i++];
			}
			if(identifier_name == "int"){
				tokens.push_back(Token(TokenType::INT));
			}
			else if(identifier_name == "io"){
				tokens.push_back(Token(TokenType::IO));
			}
			else if(identifier_name == "and"){
				tokens.push_back(Token(TokenType::AND));
			}
			else if(identifier_name == "or"){
				tokens.push_back(Token(TokenType::OR));
			}else if(identifier_name == "not"){
				tokens.push_back(Token(TokenType::NOT));
			}
			else{
				tokens.push_back(Token(TokenType::IDENTIFIER, identifier_name));
			}
		}
		else if(isdigit(line[i])){
			double num = 0;
			while(i < n && std::isdigit(line[i])){
				num = num*10 + (line[i++] - '0');
			}
			tokens.push_back(Token(TokenType::NUMBER, num));
		}else if(line[i] == '+' || line[i] == '-' || line[i] == '/' || line[i] == '*'){
			tokens.push_back(Token(TokenType::OPERATOR, line[i++]));
		}
		else if(i + 1 < n && line[i] == '<' && line[i+1] == '<'){
			tokens.push_back(Token(TokenType::DOUBLE_LT));
			i += 2;
		}
		else if(line[i] == ';'){
			tokens.push_back(Token(TokenType::SEMI_COLON));
			i++;
		}
		else if(line[i] == '='){
			i++;
			if(i < n && line[i + 1] == '='){
				tokens.push_back(Token(TokenType::EQ));
				i++;
				continue;
			}
			tokens.push_back(Token(TokenType::EQUAL));
		}else if(line[i] == '('){
			tokens.push_back(Token(TokenType::L_BRACK));
			i++;
		}else if(line[i] == ')'){
			tokens.push_back(Token(TokenType::R_BRACK));
			i++;
		}
		else if(line[i] == '<'){
			i++;
			if(i < n && line[i] == '='){
				tokens.push_back(Token(TokenType::LEQ));
				i++;
			}
			tokens.push_back(Token(TokenType::LT));
		}
		else if(line[i] == '>'){
			i++;
			if(i < n && line[i] == '='){
				tokens.push_back(Token(TokenType::GEQ));
				i++;
			}
			tokens.push_back(Token(TokenType::GT));
		}
		else if(i + 1 < n && line[i] == '!' && line[i + 1] == '='){
			i += 2;
			tokens.push_back(Token(TokenType::NEQ));
		}
		else{
			throw std::runtime_error("Undefined character found by lexer");
		}
	}
}

void traverse_tokens(const std::vector<Token>& tokens){
	for(const Token& tok : tokens){
		switch(tok.type){
			case TokenType::IDENTIFIER:
				std::cout << tok.string_data << "-> ";
				break;
			case TokenType::NUMBER:
				std::cout << tok.num_data << "-> ";
			case TokenType::OPERATOR:
				std::cout << tok.char_data << "-> ";
				break;
			case TokenType::AND:
				std::cout << "AND ->" << "->";
				break;
			case TokenType::OR:
				std::cout << "OR ->" << "->";
				break;
			case TokenType::NOT:
				std::cout << "NOT ->" << "->";
				break;
			case TokenType::INT:
				std::cout << "INT -> ";
				break;
			case TokenType::SEMI_COLON:
				std::cout  << "; -> ";
				break;
			case TokenType::EQUAL:
				std::cout << "= -> ";
				break;
			case TokenType::L_BRACK:
				std::cout << "( -> ";
				break;
			case TokenType::R_BRACK:
				std::cout << ") -> ";
				break;
			case TokenType::IO:
				std::cout << "io -> ";
				break;
			case TokenType::DOUBLE_LT:
				std::cout << "<< -> ";
				break;
			case TokenType::END_OF_FILE:
				std::cout << "EOF \n";
				break;
			default:
				std::cout << "[UNKNOWN] ->" << std::endl;
				break;
		}
	}	
}
