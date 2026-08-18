#include <stdlib.h>
#include <gc.h>
#include <error.h>
#include <parser.h>
#include <lexer.h>
#include <stdio.h>

Ks_AstNode* root = NULL;
Ks_AstNode* ast_tail = NULL;
Ks_Token* current = NULL;

Ks_AstNode* Ks_Create_AstNode(Ks_AstNode_Type type){
	Ks_AstNode* ks_node = Ks_Allocate(sizeof(Ks_AstNode));
	ks_node->type = type;
	return ks_node;
}

static void consume(){
	if(current){
		current = current->next;
	}
}

static void consume_newlines(){
	while(current && current->type == T_NEWLINE){
		consume();
	}
}

static Ks_AstNode* Ks_ParseNumber(){
	if(current && current->type == T_NUMBER){
		Ks_AstNode* node = Ks_Create_AstNode(AST_NUMBER);
		node->data.numData = current->data.numData;
		consume(); // consume the number token
		consume_newlines();
		return node;
	}else{
		return NULL;
	}
}

static Ks_AstNode* Ks_ParseMulDiv(){
	Ks_AstNode* lhs = Ks_ParseNumber();
	while(current && current->type == T_OPERATOR && (
		current->data.charData == '*' ||
		current->data.charData == '/' 		
	  )){
		Ks_AstNode* op_node = Ks_Create_AstNode(AST_OPERATOR);		
		op_node->data.charData =  current->data.charData;
		consume(); // consume the operator token
		Ks_AstNode* rhs = Ks_ParseNumber();
		op_node->left = lhs;
		op_node->right = rhs;
		lhs = op_node;
	}
	consume_newlines();
	return lhs;
}

static Ks_AstNode* Ks_ParseAddSub(){
	Ks_AstNode* lhs = Ks_ParseMulDiv();
	while(current && current->type == T_OPERATOR && (
		current->data.charData == '+' ||
		current->data.charData == '-' 		
	  )){
		Ks_AstNode* op_node = Ks_Create_AstNode(AST_OPERATOR);		
		op_node->data.charData =  current->data.charData;
		consume(); // consume the operator token
		Ks_AstNode* rhs = Ks_ParseMulDiv();
		op_node->left = lhs;
		op_node->right = rhs;
		lhs = op_node;
	}
	consume_newlines();
	return lhs;
}


static void push_node_to_root(Ks_AstNode* node){
	if(root == NULL){
		root = node;
		ast_tail = node;
	}else{
		ast_tail->next = node;
		ast_tail = node;
	}
}

void Ks_Parse(int line_no){
	current = lexer_output;
	consume_newlines();
	if(current && current->type == T_NUMBER){
		Ks_AstNode* node = Ks_ParseAddSub();
		push_node_to_root(node);		
	}else{
		Ks_Raise("Unknown token found by Ks_Parse()",line_no,RUN_TIME_ERROR);
	}
}
