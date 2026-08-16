#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <gc.h>

void Ks_Raise(char* msg,int line_no, Ks_Error type){
	printf("Kalsel Error : %s \n", msg);
	if(line_no != -1) printf("At line : %d \n", line_no);
	char* errType;
	switch(type){
		case RUN_TIME_ERROR:
			errType = "RUNTIME ERROR";
			break;
		case COMPILE_TIME_ERROR:
			errType = "COMPILE TIME ERROR";
			break;
		case FILE_ERROR:
			errType = "FILE-ORIENTED ERROR";
			break;	
	}
	printf("Error type : %s \n",errType);
	Ks_Cleanup();
	exit(1);
}
