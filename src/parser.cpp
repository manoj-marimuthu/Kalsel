#include<parser.h>
#include<lexer.h>
#include<iostream>
#include<stdexcept>
#include<memory>
#include<vector>

Token current_token;
std::vector<std::unique_ptr<AstNode>> ast_nodes{};

static void consume(){
	if(current_token_index + 1 >= tokens.size()){
		throw std::runtime_error("consumption of tokens which are not present is illegal \n");
	}
	current_token = tokens[++current_token_index];
}

static void consume_semi_colon(){
	while(current_token_index < tokens.size() && current_token.type == TokenType::SEMI_COLON){
		consume();	
	}	
}

// parse functions ,recursive descent, top-down approach)

using AstNodePtr = std::unique_ptr<AstNode>;
static AstNodePtr parseVariableAssignment();
static AstNodePtr parseExpression();

static AstNodePtr parseAtom(){
	if(current_token.type == TokenType::NUMBER){
		auto node = std::make_unique<AstNode>(AstNodeType::NUMBER, current_token.num_data);
		consume();
		return node;
	}else if(current_token.type == TokenType::INT){
		return parseVariableAssignment();
	}else if(current_token.type == TokenType::IDENTIFIER){
		auto node = std::make_unique<AstNode>(AstNodeType::IDENTIFIER, current_token.string_data);
		consume();
		return node;
	}
	else if(current_token.type == TokenType::L_BRACK){
		consume();
		auto node = parseExpression();
		if(current_token.type == TokenType::R_BRACK){
			consume();
		}else{
			throw std::runtime_error("Missing ')' for expression (detected by parser) \n");
		}
		return node;
	}
	else{
		std::cout << (int) current_token.type << std::endl;
		throw std::runtime_error("Unknown token type found by parser. Valid token needed at the beginning \n");
	}
}

static AstNodePtr parseUnary(){
	if(current_token.type == TokenType::OPERATOR && (current_token.char_data == '+' || current_token.char_data == '-')){
		char op = current_token.char_data;
		consume();
		auto node = std::make_unique<AstNode>(AstNodeType::OPERATOR, op);
		node->left = std::make_unique<AstNode>(AstNodeType::NUMBER, 0.0);
		node->right = parseUnary();
		return node;
	}
	return parseAtom();
}

static AstNodePtr parseNot(){
	if(current_token.type == TokenType::NOT){
		consume();
		auto node = std::make_unique<AstNode>(AstNodeType::NOT);
		node->child = parseExpression();
		return node;
	}
	return parseUnary();
}

static AstNodePtr parseMulDiv(){
	auto lhs = parseNot();
	while(current_token.type == TokenType::OPERATOR &&
	     (current_token.char_data == '*' || current_token.char_data == '/')
	     )
		{
			auto op_node = std::make_unique<AstNode>(AstNodeType::OPERATOR, current_token.char_data);
			consume();
			auto rhs = parseNot();
			op_node->left = std::move(lhs);
			op_node->right  = std::move(rhs);
			lhs = std::move(op_node);
		}
	return lhs;
}

static AstNodePtr parseAddSub(){
	auto lhs = parseMulDiv();
	while(current_token.type == TokenType::OPERATOR &&
 		(current_token.char_data == '+' || current_token.char_data == '-') 
	     ){	
			auto op_node = std::make_unique<AstNode>(AstNodeType::OPERATOR, current_token.char_data);
			consume();
			auto rhs = parseMulDiv();
			op_node->left = std::move(lhs);
			op_node->right  = std::move(rhs);
			lhs = std::move(op_node);
		}
	return lhs;
}

static AstNodePtr parseAnd(){
	auto lhs = parseAddSub();
	while(current_token.type == TokenType::AND){
		auto op_node = std::make_unique<AstNode>(AstNodeType::AND);
		consume();
		auto rhs = parseAddSub();
		op_node->left = std::move(lhs);
		op_node->right = std::move(rhs);
		lhs = std::move(op_node);
	}
	return lhs;
}

static AstNodePtr parseOr(){
	auto lhs = parseAnd();
	while(current_token.type == TokenType::OR){
		auto op_node = std::make_unique<AstNode>(AstNodeType::OR);
		consume();
		auto rhs = parseAnd();
		op_node->left = std::move(lhs);
		op_node->right = std::move(rhs);
		lhs = std::move(op_node);
	}
	return lhs;
}

static AstNodePtr parseExpression(){
	return parseOr();
}

static AstNodePtr parseVariableAssignment(){
	if(current_token.type == TokenType::INT){
		consume();
		if(current_token.type == TokenType::IDENTIFIER){
			const std::string variable_name = current_token.string_data;
		        consume();
			if(current_token.type == TokenType::EQUAL){
		 		consume();
				auto node = std::make_unique<AstNode>(AstNodeType::ASSIGNMENT);
				node->left = std::make_unique<AstNode>(AstNodeType::STRING, variable_name);
				node->right = parseExpression();
				if(current_token.type == TokenType::SEMI_COLON){
					consume_semi_colon();
				}
				else{
					throw std::runtime_error("Expected ';': after variable assignment \n");
				}
				return node;
			}
			else if(current_token.type == TokenType::SEMI_COLON){
				consume();
				auto node = std::make_unique<AstNode>(AstNodeType::ASSIGNMENT);
				node->left = std::make_unique<AstNode>(AstNodeType::STRING, variable_name);
				node->right = std::make_unique<AstNode>(AstNodeType::NUMBER);
				node->right->num_data = 0;
				return node;
			}
			else{
				std::cout << (int) current_token.type << std::endl;
				throw std::runtime_error("Expected '=': after variable name \n");
				
			}
		}else{
			std::cout << (int) current_token.type << std::endl;
			throw std::runtime_error("Expected Identifier: after 'int', use a valid variable name \n");
		}
	}
	else if(current_token.type == TokenType::IDENTIFIER){
		const std::string variable_name = current_token.string_data;
		consume();
		if(current_token.type == TokenType::EQUAL){
			consume();
			auto node = std::make_unique<AstNode>(AstNodeType::ASSIGNMENT);
			node->left = std::make_unique<AstNode>(AstNodeType::STRING, variable_name);
			node->right = parseExpression();
			if(current_token.type == TokenType::SEMI_COLON){
				consume_semi_colon();
			}else{
				throw std::runtime_error("Expected ';': after variable assignment \n");
			}
			return node;
		}else{
			throw std::runtime_error("Expected '=', after 'identifier' during variable initialization \n");	
		}
	}
	else{
		return nullptr;
	}
}

AstNodePtr parseIo(){
	if(current_token.type == TokenType::IO){
		consume();
		if(current_token.type == TokenType::DOUBLE_LT){
			AstNodePtr node = std::make_unique<AstNode>(AstNodeType::IO_OUT);		
			AstNode* tail = node.get();
			while(current_token.type == TokenType::DOUBLE_LT){
				consume();
				auto to_print = parseExpression();
				if(!to_print){
					throw std::runtime_error("Expected an expression after '<<' \n");
				}
				tail->next = std::move(to_print);
				tail = tail->next.get();
			}
			consume_semi_colon();
			return node;
			
		}else{
			throw std::runtime_error("Expected a valid token after 'io', use '<<' to print \n");
		}
	}
	return nullptr;
}

void Parse(){
	current_token = tokens[0];
	while(current_token.type != TokenType::END_OF_FILE){
		consume_semi_colon();
		if(current_token.type == TokenType::INT  || 
		   current_token.type == TokenType::IDENTIFIER
		   )
		{
			auto node = parseVariableAssignment();
			ast_nodes.push_back(std::move(node));
		}
		else if(current_token.type == TokenType::IO){
			auto node = parseIo();
			ast_nodes.push_back(std::move(node));
		}
		else {
			std::cout << (int) current_token.type << std::endl;
			throw std::runtime_error("Unknown token type found by parser. Valid token type needed \n");
		}	
	}
}
