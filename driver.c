//==========================================================//
//		PROJECT: Compiler Construction						//						
//		Group Number : 15                                   //
//		Team Number : 1										//
//		Team Members:										//
//			1. Divyanshu Goyal		2011B4A7795P			//
//			2. Abhishek Modi		2011B4A7331P			//
//															//
//==========================================================//

#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include "link.h"
#include "lexerStructure.h"
#include "parseStructure.h"
#include "symbolTableStructure.h"

char LexicalUnits[10000][100];
char tokenValue[10000][100];
int line[10000];

int main(){
	printf("******* INTIATING COMPILER ***********\n");
	//printf("Choose Relevant stage of compiler:\n");
	//printf("1. Lexical Analysis\n");
	//printf("2. Parsing\n");
	//printf("3. Semantic Analysis\n");
	//printf("4. Code Generation\n");
	
	int choice,i;
	char c;
	if(1){
		
		hashTable* T = build_hash();   	        // build the hash table to maintain state of tokens
		printStoreHash(T); 			// store hashTable in a file
		Dnode* DFA = buildDFA();
		memset(LexicalUnits,'\0',sizeof(LexicalUnits));

		lexerCode(DFA,T,LexicalUnits,tokenValue,line);
		printf("\nLexer tokens have been generated in Lexer Tokens.txt\n");
		pnode* root=NULL;
		root=parse(LexicalUnits,tokenValue,line);
		printf("\nParse tree generated in parseTree.txt\n");
		pnode* ast=getAST(root);
		printf("\nAbstarct Syntax Tree Generated in AbstractSyntaxTree.txt\n");
		Master *sym = (Master*)malloc(sizeof (Master)); 
		getSymbolTable(sym,ast);
		printf("\nSymbol Table Generated in symbolTable.txt\n");
		GenerateCode(sym,ast);
		printf("\nCode Generation Successfull\n\n");
	} 
	return 0;

}

