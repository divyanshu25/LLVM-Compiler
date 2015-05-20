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
#include "lexerStructure.h"
#include <unistd.h>
char err[] = {'N','O','T','F','O','U','N','D','\0'};
//=============================================//
void insertDFA(Dnode* root, char ch , int st, int pos){	
	//printf("%c %d\n",ch,st);
	root -> ptr[pos] = (Dnode*)malloc(sizeof(Dnode));
	Dnode* temp = root -> ptr[pos];
	temp -> val =  ch;
	temp -> isFinal = st;
	int i;
	for(i = 0 ; i< 32 ; i++){
		temp -> ptr[i] = NULL;
	}
}
//=====================================================//
void insertDFAchild(Dnode* start, char* buff, int pos, int chpos){
	int len = strlen(buff);
	//printf("%d\n",len);
	Dnode* crawl = start -> ptr[pos];
	crawl -> ptr[chpos] = (Dnode*)malloc(sizeof(Dnode));
	crawl = crawl -> ptr[chpos];
	int i ,j;
	for(i = 0 ; i< len - 1 ; i++){
		crawl -> val = buff[i];
		crawl -> isFinal = -1;
		for(j = 0 ; j< 32 ; j ++)
			crawl -> ptr[j]= NULL;
		crawl -> ptr[0] = (Dnode*)malloc(sizeof(Dnode));
		crawl = crawl -> ptr[0];
	}
	crawl -> val = buff[len-1];
	crawl -> isFinal = 1;
	for(j = 0 ; j< 32 ; j ++)
		crawl -> ptr[j]= NULL;
		
}
//====================================================//
Dnode* buildDFA(){
	FILE* fp = fopen("DFA.txt","r");
	Dnode* start = (Dnode*)malloc(sizeof(Dnode));
	int fch,i,st,nch,j;
	char ch,bl;
	char buff[20];
	for(i = 0 ; i < 32 ; i++){
		start -> ptr[i] = NULL;
	}
	
	fscanf(fp,"%d",&fch);
	fscanf(fp,"%c",&bl);
	//printf("%d\n",fch);
	for(i  = 0 ; i< fch ; i++){
		fscanf(fp,"%c %d",&ch,&st);
		fscanf(fp,"%c",&bl);
		insertDFA(start,ch,st,i);
	}
	for(i = 0 ; i< fch ; i++){
		memset(buff,'\0',sizeof(buff));	
		fscanf(fp,"%d",&nch);
		fscanf(fp,"%c",&bl);
		if(nch == 0)
			continue;
		for(j = 0 ; j < nch ; j ++){
			fscanf(fp,"%s",buff);
			fscanf(fp,"%c",&bl);
			//printf("%s\n",buff);
			insertDFAchild(start,buff,i,j);
		}
	}
return start;
}
//===============================================//
hashTable* init(){
	hashTable *T = (hashTable*)malloc(sizeof(hashTable));
	T-> size = 100;
	int i;
	for(i = 0; i< T -> size ; i++){
		T -> table[i] =NULL;
	}
return T;
}
//===============================================//
int getAscii(char *s){
	int val = 0;
	int i = 0;
	while(s[i] != '\0'){
		val += ((((int)s[i])*17)/3);
		i++;
	}
	return (val+i)%100;
}
//============================================//
void insert(hashTable* T,int pos, char *str, char *token){
	Node* n = (Node*)malloc(sizeof(Node));
	n -> next = NULL;
	strcpy(n -> str, str);
	strcpy(n -> token, token);
	if((T-> table[pos]) == NULL){
		(T-> table[pos]) = n;
	}
	else{
		n -> next = (T-> table[pos]);
		(T-> table[pos]) = n;
	}
}
//=========================================================//
void printStoreHash(hashTable *T){
	FILE* fp;
	fp = fopen("hashTable.txt","w+");
	int i;
	Node *crawl;
	for(i= 0 ; i< T -> size ; i++){
		fprintf(fp,"%d->	",i);
		crawl = T-> table[i];
		while(crawl != NULL){
			fprintf(fp," ~%s %s~ ",crawl -> str, crawl -> token);
			crawl = crawl -> next;
		}
		fprintf(fp,"\n",NULL);
	}
}
//=================================================================//
char* lookup(char *str,hashTable* T){
	int val = getAscii(str);
	Node* crawl = T -> table[val];
	while(crawl != NULL){
		if(strcmp(str, (crawl -> str)) == 0){
			return crawl -> token;
		}
			crawl = crawl -> next;
	}
return err;

}
//=========================================================//
hashTable* build_hash(){
	hashTable *T = init();
	FILE* fp;  				// get file pointer to token file
	fp =  fopen("tokens.txt","r+"); 	// openfile
	if(fp == NULL){
		printf("File Not Open\n");
		return;	
	}
	char str[20],token[20];
	memset(str,'\0',sizeof(str));
	memset(token,'\0',sizeof(token));
	while(fscanf(fp,"%s%s",str,token) != EOF){
		int val = getAscii(str); 		// get ASCII value of the token 
		//printf("%s %d\n",str,val);
		insert(T, val,str,token);	  	// insert into hash table
	}
	//printStoreHash(T);
	return T;
}
//===============================================================//
int isStrL(char *str){
		int l = strlen(str);
		if(str[l-1] == '"')
			return 1;
return 0;
}
//========================================================//
int isCharL(char *str){
	int l = strlen(str);
	if(l == 3 && str[l-1] == 39)
		return 1;
return 0;
}
//========================================================//
int isNUM(char* str){
	int l = strlen(str);
	int i;
	int flag = 0;
	for(i= 0 ; i< l ; i++){
		if(str[i] < 48 || str[i] > 57){
			flag =1;
			break;
		}
	}
	if(flag == 1)
		return 0;
	else
		return 1;
}
//========================================================//
int isFLOAT(char *str){
	int dot =0;
	int l = strlen(str);
	int i;
	int flag = 0;
	for(i = 0; i< l ; i++){
		if(str[i] == 46){
			if(dot == 0)
				dot++;
			else{
				flag =1;
				break;
			}
		}
		else if(str[i] <48 || str[i] > 57){
			flag = 1;
			break;
		}
	}
	if(flag == 1)
		return 0;
	else
		return 1;
}
//=========================================================//
int isID(char *str){
	//printf(".. %s\n",str);
	int l = strlen(str);
	int flag = 0;
	if( (str[0] >= 97 && str[0] <= 122) || (str[0] >= 65 && str[0] <= 90) ){
		flag = 0;
	}
	else{
		return 0;
	}
	int i;
	for(i = 1 ; i< l ; i++){
		if( (str[i] >= 97 && str[i] <= 122) || (str[i] >= 65 && str[i] <= 90) || (str[i] >= 48 && str[i] <= 57) ){
			continue;
		}
		else{
			flag = 1;
			break;
		}
		
	}
	if(flag == 1)
		return 0;
	else return 1;
	
}
//=========================================================//
void lexerCode(Dnode* start, hashTable* T, char LexicalUnits[][100], char tokenValue[][100], int lin[]){
	Dnode* temp;
	int i = 0;
	int test;
	int unit = 0;
	char fileName[100];
	memset(fileName,'\0',sizeof(fileName));
	char Buff[10000];
	memset(Buff,'\0',sizeof(Buff));
	printf("Enter Test Case Number: ");
	scanf("%d",&test);
	sprintf(fileName,"Sample%d.txt",test);
	FILE *fp = fopen(fileName,"r");
	FILE* tn = fopen("Lexer Tokens.txt","w+");
	while(fscanf(fp,"%c",&Buff[i]) != EOF){
		i++;
	}
	//printf("%s\n",Buff);
	Buff[i] = '\0';
	char GB[1000];
	int Gptr = 0;
	memset(GB,'\0',sizeof(GB));
	char* curr;
	char* next;
	curr = Buff;
	next = Buff+1;
	int line = 1;
	Dnode* crawl = start;
	int flag = 0;
	while((*curr) != '\0'){
		flag = 0;
		while((((*curr)) == ' ')||( ((*curr)) == '\n' ) || (((*curr)) == '\t')){
			if(*curr == '\n')
				line++;
			curr++;
		}
		if( (crawl != start) && (
			((*curr) == ';') || ((*curr) == ',') || ((*curr) == '*') || ((*curr) == '/') || ((*curr) == '+') || ((*curr) == '-') || ((*curr) == '(') || ((*curr) == ')') || ((*curr) == '[') || ((*curr) == ']') || ((*curr) == '{') || ((*curr) == '}') || ((*curr) == '<') || ((*curr) == '>')|| ((*curr) == '.') || ((*curr) == ':') || ((*curr) == '=')
		
		)){
			
			char *tokn = lookup(GB,T);
			
			if(!strcmp(tokn,"NOTFOUND")){
				if(GB[0] == '"'){
					if(isStrL(GB)){
						fprintf(tn,"%s	STRL\n",GB);
						strcpy(LexicalUnits[unit],"STRL");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				else if(GB[0] == 39){
					if(isCharL(GB)){
						fprintf(tn,"%s	CHARL\n",GB);
						strcpy(LexicalUnits[unit],"CHARL");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				else if(GB[0] >= 48 && GB[0] <= 57){
					if(isNUM(GB)){
						fprintf(tn,"%s	NUM\n",GB);
						strcpy(LexicalUnits[unit],"NUM");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else if(isFLOAT(GB)){
						fprintf(tn,"%s	FLOAT\n",GB);
						strcpy(LexicalUnits[unit],"FLOAT");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				else{
					 if(isID(GB)){
						fprintf(tn,"%s	ID\n",GB);
						strcpy(LexicalUnits[unit],"ID");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					 else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				
					
			}
			memset(GB,'\0',sizeof(GB));
			crawl = start;
			Gptr = 0;
			continue;	
		}
		SKIP:
		for(i = 0 ; i< 32 ; i++){
			if(crawl -> ptr[i] != NULL && (crawl -> ptr[i] -> val) == (*curr)){
				flag = 1;
				crawl = crawl -> ptr[i];
				GB[Gptr] = (char)(*curr);
				Gptr++; 
				break;
			}
		}
		
		char cc = *curr;
		if(flag == 0){
			while((*curr != '\0') &&
			((cc != ';' && cc != ',' && cc != '*'  && cc != '/' && cc != '+' && cc != '-' && cc != '(' && cc != ')' && cc != '[' && cc != ']' && cc != '{' && cc != '}' && cc != '<' && cc != '>' && cc!= ' '&& cc != '\t' && cc != '\n' && cc != ':' && cc != '='))
			){
			if( cc == '.'){
				char *pre = curr;
				pre--;
				char *ne = curr;
				ne++;
				if((*pre) >= 48 && (*pre)<=57 && (*ne) >= 48 && (*ne) <= 57){
					GB[Gptr] = (char)(*curr);
					Gptr++;
					curr++;
					cc = *curr;
				}
				else
					break;
			}
			if(cc == '\n')
				line++;
			GB[Gptr] = (char)(*curr);
			Gptr++;
			curr++;
			cc = *curr;
			//printf("%s %c\n",GB,cc);
			//sleep(1);
		}
			
			if(*curr == '\0')
				return;
			//printf("%s %c\n",GB,*(curr));
			char *tokn = lookup(GB,T);
			if(!strcmp(tokn,"NOTFOUND")){
				if(GB[0] == '"'){
					if(isStrL(GB)){
						fprintf(tn,"%s	STRL\n",GB);
						strcpy(LexicalUnits[unit],"STRL");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				else if(GB[0] == 39){
					if(isCharL(GB)){
						fprintf(tn,"%s	CHARL\n",GB);
						strcpy(LexicalUnits[unit],"CHARL");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				else if(GB[0] >= 48 && GB[0] <= 57){
					if(isNUM(GB)){
						fprintf(tn,"%s	NUM\n",GB);
						strcpy(LexicalUnits[unit],"NUM");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else if(isFLOAT(GB)){
						fprintf(tn,"%s	FLOAT\n",GB);
						strcpy(LexicalUnits[unit],"FLOAT");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				else{
					 if(isID(GB)){
						fprintf(tn,"%s	ID\n",GB);
						strcpy(LexicalUnits[unit],"ID");
						strcpy(tokenValue[unit],GB);
						lin[unit] = line;
						unit++;
					}
					 else{
						fprintf(tn,"%s	ERROR\n",GB);
						printf("Error in '%s' at line No. %d\n",GB,line);
					}
				}
				
					
			}
			memset(GB,'\0',sizeof(GB));
			crawl = start;
			Gptr = 0;
			continue;
		
		}
		if(crawl -> isFinal == 1){
			for(i = 0 ; i < 32 ; i++){
				if((crawl -> ptr[i] != NULL) && (crawl -> ptr[i] -> val == *(curr+1))&&(crawl -> ptr[i] -> isFinal == 1)){
					curr++;
					next++;
					char c = 1;
					//printf("%c\n",c);
					GB[Gptr] = (char) *(curr);
					Gptr++;
					break;
				}
			}
			char *tokn = lookup(GB,T);
			//printf("%s\n",tokn);
			fprintf(tn,"%s	%s\n",GB,tokn);
			strcpy(LexicalUnits[unit],tokn);
			strcpy(tokenValue[unit],GB);
			lin[unit] = line;
			unit++;
			memset(GB,'\0',sizeof(GB));
			crawl = start;
			Gptr = 0;	
		}
		curr++;		
	}
	
}
//=======================================================================================//
