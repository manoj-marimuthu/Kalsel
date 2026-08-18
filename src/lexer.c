#include <token.h>
#include <lexer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <error.h>

Ks_Token* lexer_output = NULL;
Ks_Token* lexer_tail   = NULL;

static void push_into_lexer_output(Ks_Token* tok){
	if(lexer_output == NULL){
		lexer_output = tok;
		lexer_tail = tok;
	}else{
		lexer_tail->next = tok;
		lexer_tail = tok;
	}
}
void line_lexer(char* line,int line_no){
	int i = 0;
	int n = strlen(line);
	// reset token streams
	lexer_output = NULL;
	lexer_tail = NULL;
	while(i < n){
		Ks_Token* tok = NULL;
		if(isspace(line[i])){
			i++;
			continue;
		}else if(isdigit(line[i])){
			// if it is a number (single/multi digit)
			int temp = 0;
			while(i < n && isdigit(line[i])){
				temp = temp* 10 + (line[i] - '0');
				i++;
			}
			tok = Ks_Create_Token(T_NUMBER);
			tok->data.numData = temp;
		}else if(line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/'){
			tok = Ks_Create_Token(T_OPERATOR);
			tok->data.charData = line[i];
			i++;
		}
		else{
			Ks_Raise("lexer found an unknown token type",line_no,RUN_TIME_ERROR);
		}
		push_into_lexer_output(tok);	
	}
	push_into_lexer_output(Ks_Create_Token(T_NEWLINE));
}

void traverse_lexer_output(){
	Ks_Token* curTok = lexer_output;
	while(curTok){
		switch(curTok->type){
			case T_NUMBER:
				printf("%d -> ", curTok->data.numData);
				break;
			case T_OPERATOR:
				printf("%c -> ", curTok->data.charData);
				break;
			case T_NEWLINE:
				printf("%s -> ","NEWLINE");
			default:
				break;
		}
		curTok = curTok->next;
	}
}
