//==========================================================//
//		PROJECT: Compiler Construction						//						
//		Group Number : 15                                   //
//		Team Number : 1										//
//		Team Members:										//
//			1. Divyanshu Goyal		2011B4A7795P			//
//			2. Abhishek Modi		2011B4A7331P			//
//															//
//==========================================================//

#ifndef _LINKH_
#define _LINKH_
#include "lexerStructure.h"
#include "parseStructure.h"
#include "symbolTableStructure.h"

extern hashTable* build_hash(); 
extern void printStoreHash();
extern char* lookup();
extern Dnode* buildDFA();
extern void lexerCode();
extern pnode* parse();
extern pnode* getAST();
extern Master* getSymbolTable();
extern void GenerateCode();
#endif
