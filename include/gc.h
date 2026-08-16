#ifndef GC_H
#define GC_H
#include <stdlib.h>

typedef struct Ks_Mem_Node{
	void* ptr;
	struct Ks_Mem_Node* next;
} Ks_Mem_Node;

extern Ks_Mem_Node* program_memory;


void* Ks_Allocate(size_t n);

void Ks_Cleanup();

#endif
