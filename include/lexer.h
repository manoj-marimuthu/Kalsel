#ifndef LEXER_H
#define LEXER_H

#include <token.h>
#include <stdlib.h>


extern Ks_Token* lexer_output;
extern	Ks_Token* lexer_output_tail;

void line_lexer(char* line,int line_no);
void traverse_lexer_output();
#endif
