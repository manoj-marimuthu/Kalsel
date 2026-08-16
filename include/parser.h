#ifndef PARSER_H
#define PARSER_H

typedef enum{
	AST_NUMBER,
	AST_OPERATOR
} Ks_AstNode_Type;

typedef struct Ks_AstNode{
	Ks_AstNode_Type type;
	union{
		int numData;
		char charData;
	} data;
	struct Ks_AstNode* left;
        struct Ks_AstNode* right;	
	struct Ks_AstNode* next;
} Ks_AstNode;

extern Ks_AstNode* root;

Ks_AstNode* Ks_Create_AstNode(Ks_AstNode_Type type);
void Ks_Parse(int line_no);

#endif
