#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<gc.h>
#include<token.h>
#include<lexer.h>
#include<parser.h>

int eval_test(Ks_AstNode* exp){
	if(exp->type == AST_NUMBER){
		return exp->data.numData;	
	}else{
		switch(exp->data.charData){
			case '+':
				return eval_test(exp->left) + eval_test(exp->right);	
			case '-':
				return eval_test(exp->left) - eval_test(exp->right);
			case '*':
				return eval_test(exp->left) * eval_test(exp->right);
			case '/':
				return eval_test(exp->left) / eval_test(exp->right);
		}
	}
	return 0;
}
int main(){
	line_lexer("1 + 3 - 90",1);
	traverse_lexer_output();
	Ks_Parse(1);
	int result = eval_test(root);
	printf("Test : %d \n",result);
	Ks_Cleanup();
	return 0;
}
