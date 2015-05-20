#ifndef LEXERSTRUCTREH_
#define LEXERSTRUCTREH_
//======================================//
typedef struct node{
	char str[20];
	char token[20];
	struct node* next;
}Node;
//======================================//
typedef struct hash{
	int size;
	Node *table[100];	
}hashTable;
 
//======================================//
typedef struct DFANode{
	char val;
	int isFinal;
	struct DFANode* ptr[32];
}Dnode;
//======================================//
typedef struct StateChecker{
	int start;
	int isNum;
	int isFloat;
	char Buff[100];
	int ptr;
}Schecker;
//======================================//
#endif
