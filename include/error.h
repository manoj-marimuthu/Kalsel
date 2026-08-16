#ifndef ERROR_H
#define ERROR_H

typedef enum {
	RUN_TIME_ERROR,
	FILE_ERROR,
	COMPILE_TIME_ERROR,
} Ks_Error;

void Ks_Raise(char* msg,int line_no,Ks_Error type);

#endif
