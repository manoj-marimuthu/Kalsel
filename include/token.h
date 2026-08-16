#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
	T_NUMBER,
	T_OPERATOR,
	T_NEWLINE
} Ks_Token_Type;

typedef struct Ks_Token{
	Ks_Token_Type type;
	union{
		int numData;
		char charData;
	} data;
	struct Ks_Token* next;
} Ks_Token;

Ks_Token* Ks_Create_Token(Ks_Token_Type type);

#endif
