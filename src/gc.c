#include <stdlib.h>
#include <gc.h>
#include <error.h>

Ks_Mem_Node* program_memory = NULL;

void* Ks_Allocate(size_t n){
	Ks_Mem_Node* ks_node = malloc(sizeof(Ks_Mem_Node));
	if(ks_node == NULL){
		Ks_Raise("Memory allocation failed at Ks_Allocate(), during KsNode allocation",-1,COMPILE_TIME_ERROR);	
	}
     	ks_node->next = NULL;
	
	void* ptr = malloc(n);
	if(ptr == NULL){
		Ks_Raise("Memort allocation failed at Ks_Allocate(), during KsNode.ptr allocation",-1,COMPILE_TIME_ERROR);
	}
	ks_node->ptr = ptr;
       // store allocations in the global pool
	ks_node->next = program_memory;
	program_memory = ks_node;
	return ks_node->ptr;
}

void Ks_Cleanup(){
	Ks_Mem_Node* curMemNode = program_memory;
	Ks_Mem_Node* nextMemNode = NULL;
	while(curMemNode != NULL){
		nextMemNode = curMemNode->next;
		if(curMemNode){
			if(curMemNode->ptr) free(curMemNode->ptr);
			free(curMemNode);
		}
		curMemNode = nextMemNode;
	}
}
