#include <token.h>
#include <gc.h>

Ks_Token* Ks_Create_Token(Ks_Token_Type type){
	Ks_Token* token = Ks_Allocate(sizeof(Ks_Token));
	token->type = type;
	token->next = NULL;
	return token;
}
