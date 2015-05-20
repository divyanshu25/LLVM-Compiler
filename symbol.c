//==========================================================//
//		PROJECT: Compiler Construction						//						
//		Group Number : 15                                   //
//		Team Number : 1										//
//		Team Members:										//
//			1. Divyanshu Goyal		2011B4A7795P			//
//			2. Abhishek Modi		2011B4A7331P			//
//															//
//==========================================================//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTableStructure.h"
#include "lexerStructure.h"
#include "parseStructure.h"
//==============================================================================//
char keyWords[][100] = {"length","toUpper","reverse","size"};
//=============================================================================//
int inKeyWords(char *nm){
	int i = 0;
	for( i = 0 ; i < 4 ; i++){
		if(!strcmp(keyWords[i] , nm)){
			return 1;
		}
	}
	return 0;
}
//=============================================================================//
STable* getNewTable() {
	STable* ret = (STable* )malloc (sizeof(STable));
	ret->numRecords=0;
	ret -> numPara = 0;
	ret -> returnType = 0;
	return ret;
}
//===========================================================================//
int lookUp(STable* tab, char nm[], int scope){
	
	int i;
	/*for(i = tab-> numRecords ; i >= 0 ; i--){
		printf(">>> %s %d ..",(tab->rec[i]).varName, (tab->rec[i]).scope );
	}*/
	for(i = tab-> numRecords ; i >= 0 ; i--){
		if( (!strcmp((tab->rec[i]).varName , nm)) && (tab->rec[i]).scope == scope )
			return i;
	}
	return -1;
}
//==========================================================================//
int lookUpPrev(STable* tab, char nm[], int scope){
	int i;
	/*for(i = tab-> numRecords ; i >= 0 ; i--){
		printf("%s %d ..",(tab->rec[i]).varName, (tab->rec[i]).scope );
	}*/
	//printf("\n");
	for(i = tab-> numRecords ; i >= 0 ; i--){
		if( (!strcmp((tab->rec[i]).varName , nm)) && ((tab->rec[i]).scope) <= scope)
			return i;
	}
	return -1;
}
//===========================================================================//
STable* lookUpFunc(Master* sym, char nm[]){
	int i;
	int flag  = 0;
	for(i = 0 ; i< sym  -> numFunc ; i++){
		if(!strcmp (sym -> func[i] -> func_name , nm) ){
			//printf("%s..\n", sym -> func[i] -> func_name);
			flag = 1;
			return 	sym -> func[i];
		}
	}
	if(flag  == 0){
		printf("Semantic Error: No function by name '%s' decalred.\n", nm);
		exit(0);
	}
}
//===========================================================================//
void insertR(STable* tab, char nm[], int scope, int line){
	//printf("<<<%s>>>\n",nm);
	tab->rec[tab -> numRecords].refCnt = 0;
	strcpy((tab->rec[tab -> numRecords]).varName , nm);
	strcpy((tab->rec[tab -> numRecords]).func_name , tab -> func_name);
	(tab->rec[tab -> numRecords]).scope =  scope;
	(tab->rec[tab -> numRecords]).lineNo =  line;
	tab -> numRecords ++;
	return;
}
//============================================================================//
void insertRDec(STable* tab,char nm[],char value[],int type,int scope, int line, int mut, char class[]){
	//printf("%s> %s > %d > %d > %d\n",nm,value,type,scope,mut);
	if(!strcmp(class,"Parameter")){
		if(type == 22)
			tab -> typePara[tab->numPara] = 49;
		else if(type == 23)
			tab -> typePara[tab->numPara] = 19;
		else if(type == 24)
			tab -> typePara[tab->numPara] = 31;
		else if(type == 50)
			tab -> typePara[tab->numPara] = 30;
		else
			tab -> typePara[tab->numPara] = type;	
		(tab -> numPara) ++;
	}
	//printf("%s %d %s\n", tab-> func_name, tab -> numPara, nm);
	tab->rec[tab -> numRecords].refCnt = 0;
	(tab->rec[tab -> numRecords]).isStr = 0;
	strcpy((tab->rec[tab -> numRecords]).varName , nm);
	strcpy((tab->rec[tab -> numRecords]).func_name , tab -> func_name);
	strcpy((tab->rec[tab -> numRecords]).class , class);
	(tab->rec[tab -> numRecords]).scope =  scope;
	(tab->rec[tab -> numRecords]).lineNo =  line;
	if(type == 19)
		(tab->rec[tab -> numRecords]).type = 23;
	//******************************* Handling String Data structure *********************//
	else if(type == 31){
		(tab->rec[tab -> numRecords]).type = 24;
		(tab->rec[tab -> numRecords]).isStr = 1;
		(tab->rec[tab -> numRecords]).str = (string*)malloc(sizeof(string));
		strcpy(((tab->rec[tab -> numRecords]).str) -> arr,value);
		int i;
		int j =  strlen(value);
		((tab->rec[tab -> numRecords]).str) -> rev_arr[j] = '\0';
		((tab->rec[tab -> numRecords]).str) -> Upper_arr[j] = '\0';
		
		for(i = 0 ; i < j ; i++){
			((tab->rec[tab -> numRecords]).str) -> rev_arr[j-i-1] = value[i];
		}
		for(i = 0 ; i < j ; i++){
			if(value[i] >= 'a' && value[i] <= 'z')
				((tab->rec[tab -> numRecords]).str) -> Upper_arr[i] = value[i] - 32;
			else
				((tab->rec[tab -> numRecords]).str) -> Upper_arr[i] = value[i];
		}
		
		//printf("%s %s \n", ((tab->rec[tab -> numRecords]).str) -> rev_arr, ((tab->rec[tab -> numRecords]).str) -> Upper_arr);
		((tab->rec[tab -> numRecords]).str) -> len = strlen(value);
		strcpy(((tab->rec[tab -> numRecords]).str) -> name,nm);
		((tab->rec[tab -> numRecords]).str) -> flagRev = 0;
		((tab->rec[tab -> numRecords]).str) -> flagUpp = 0;
	}
	else if(type == 30)
		(tab->rec[tab -> numRecords]).type = 50;
	else if(type == 31)
		(tab->rec[tab -> numRecords]).type = 24;
	else if(type == 49)
		(tab->rec[tab -> numRecords]).type = 22;
	
	else
		(tab->rec[tab -> numRecords]).type = type;
		
	strcpy((tab->rec[tab -> numRecords]).value,value);
	
	if(mut != 2){
		tab->rec[tab -> numRecords].isMutable = 1;
	}
	else if(mut == 2){
		tab->rec[tab -> numRecords].isMutable = 0;
	}
	tab -> numRecords ++;
	
	return;
}
//============================================================================//
void updateRef(STable* tab,int ind, int line){
	//printf("%d %s\n",line, (tab->rec[ind]).varName);
	int i = (tab->rec[ind]).refCnt;
	(tab->rec[ind]).refList[i] = line;
	(tab->rec[ind]).refCnt++;
}
//===========================================================================//
void updateReftoStr(STable* tab,int ind, int line,char func[]){
	//printf("%d %s\n",line, (tab->rec[ind]).varName);
	int i = (tab->rec[ind]).refCnt;
	(tab->rec[ind]).refList[i] = line;
	(tab->rec[ind]).refCnt++;
	if(!strcmp(func,"reverse")){
		((tab->rec[ind]).str)-> flagRev = 1;
	}
	else if(!strcmp(func,"toUpper")){
		((tab->rec[ind]).str)-> flagUpp = 1;
	}
	else if(!strcmp(func,"size")){
	
	}
	else{
		printf("Semantic Error: '%s' is not supported by string at line no %d \n",func,line);
	}
}
//===========================================================================//
void handleParList(STable* tab, pnode* ast, int scope){
	int i;
	int cnt = 0;
	int type;
	char nm[100];
	int flag  = 0;
	for(i = 0 ; i< 200 ; i++){
		if( (ast -> child[i] != NULL) && (ast -> child[i] -> val == 7) && (cnt % 2 == 0) ){
			strcpy(nm , ast -> child[i] -> name);
			cnt++;
			flag = 0;
		}
		else if( (ast -> child[i] != NULL)  &&  ( ast -> child[i] -> val == 49 ||ast -> child[i] -> val == 21 || 
			ast -> child[i] -> val == 22|| ast -> child[i] -> val == 23 || 
			ast -> child[i] -> val == 24 || ast -> child[i] -> val == 50 ) && (cnt%2 == 1) ){
			type = ast -> child[i] -> val;
			//printf("type: %d\n", type);
			cnt++;
		}
		if( (cnt%2 == 0) && flag == 0){
			flag =1;
			int res = lookUp(tab, nm, scope);
			if(res != -1){
				printf("Error at '%s' on line no: %d\n",nm,ast->line);
				exit(0);
			}
			else{
				//void insertRDec(STable* tab,char nm[],char value[],int type,int scope, int line, int mut, char class[]){
				insertRDec(tab,nm," ",type,scope,ast->line,0,"Parameter");
			}
		}
	}
}
//===========================================================================//
int ValidateOpExp(pnode * root, int type, STable* tab, int scope){
		pnode* left =NULL;
		pnode* right =NULL;
		int i,cnt = 0;
		for(i = 0 ; i < 200 ; i++){
			if(root -> child[i] !=NULL && cnt == 0){
				left = root -> child[i];
				cnt++;
			}
			else if(root -> child[i] != NULL && cnt == 1){
				right = root -> child[i];
				break;
			}
		}
		if(left -> val == 7){
			//printf("%s\n", left -> name);
			int res =  lookUp(tab, left -> name , scope);
			int res2 =  lookUpPrev(tab, left -> name , scope);
			if(res == -1 && res2 == -1){
				printf("Semantic Error: Variable '%s' is not decalred at line no: %d\n", left -> name , left -> line);
				exit(0);
			}
			else if(res!= -1){
				if((tab->rec[res]).type != type){
					printf("Semantic Error: Variable '%s' is not of compatible type to left hand side at line no: %d\n",left->name,left->line);
					exit(0);
				}
			}
			else if(res2!= -1){
				if((tab->rec[res2]).type != type){
					printf("Semantic Error: Variable '%s' is not of compatible type to left hand side at line no: %d\n",left->name,left->line);
					exit(0);
				}
			}
		}
		else if((left -> val == 19 && type != 23) ||(left -> val == 49 && type != 22) ){
			printf("Semantic Error: Variable '%s' is not of compatible type to left hand side at line no: %d\n",left->name,left->line);
			exit(0);
		}
		if(right -> val == 7){
			//printf("%s\n", right -> name);
			int res =  lookUp(tab, right -> name , scope);
			int res2 =  lookUpPrev(tab, right -> name , scope);
			if(res == -1 && res2 == -1){
				printf("Semantic Error: Variable '%s' is not decalred at line no: %d\n", right -> name , right -> line);
				exit(0);
			}
			else if(res!= -1){
				if((tab->rec[res]).type != type){
					printf("Semantic Error: Variable '%s' is not of compatible type to left hand side at line no: %d\n",right->name,right->line);
					exit(0);
				}
			}
			else if(res2!= -1){
				if((tab->rec[res2]).type != type){
					printf("Semantic Error: Variable '%s' is not of compatible type to left hand side at line no: %d\n",left->name,right->line);
					exit(0);
				}
			}
		}
		else if((right -> val == 19 && type != 23) ||(right -> val == 49 && type != 22) ){
			printf("Semantic Error: Variable '%s' is not of compatible type to left hand side at line no: %d\n",right->name,right->line);
			exit(0);
		}
		else if(right -> val == 32|| right -> val == 33 || right -> val == 34 || right ->val == 35){
			ValidateOpExp(right , type, tab, scope);
		}
		return 1;
}
//===========================================================================//
void handleDeclarations(STable* tab, pnode* ast,int scope,int declare,Master* sym){
	pnode* left =NULL;
	pnode* right =  NULL;
	int leftSize, rightSize;
	int cnt = 0,i,j,posl,posr;
	char vh[100];
	if(declare > 0){
		for(i= 0 ; i < 200 && cnt < 2; i++){
			if(ast -> child[i] != NULL && cnt == 0){
				left = ast->child[i];
				cnt++;
			}
			else if(ast ->child[i] != NULL && cnt == 1){
				right = ast->child[i];
				cnt++;
			}
		}
		leftSize = left -> numChild;
		rightSize = right -> numChild;
		//printf("...........%s %d %s %d\n", left -> name , leftSize, right -> name, rightSize);
		if(leftSize == rightSize && leftSize == 0){
			int res = lookUp(tab,left -> name,scope);
			if(res != -1){
				printf("Semantic Error:\n'%s' Already Declared at line: '%d'\n",
							left -> name,ast->line);
				exit(0);
			}
			else{
			// TODO: handle right side ass if directly value or indirect value
			insertRDec(tab,left -> name,right-> name,
					   right -> val,scope ,left-> line,declare,"Var");
					   
			}
		}
		else if(leftSize == rightSize && leftSize != 0){
			for(i = 1 ; i<= leftSize ; i++){
				cnt = 0;
				for(j = 0 ; j< 200 ; j++){
					if(left -> child[j] != NULL){
						cnt++;
						if(cnt == i){
							posl = j;
						}
					}
				}
				cnt = 0;
				for(j = 0 ; j< 200 ; j++){
					if(right -> child[j] != NULL){
						cnt++;
						if(cnt == i){
							posr = j;
						}
					}
				}
				//printf("%s %s\n", left -> child[posl] -> name, right ->child[posr] -> name );
				int res = lookUp(tab,left -> child[posl] -> name,scope);
				if(res != -1){
					printf("Semantic Error:\n'%s' Already Declared at line: '%d'\n",
								left -> child[posl] -> name,ast->line);
					exit(0);
				}
				else
				insertRDec(tab,left -> child[posl] -> name,right ->child[posr] -> name,
					   right ->child[posr] -> val,scope ,left-> child[posl] -> line,declare,"Var");	
			}
			
		}
		else if(leftSize != rightSize && leftSize == 0 ){
			memset(vh , '\0', sizeof(vh));
			cnt = 0;
			int tp;
			int type;
			int flag = 0;
			//printf("%s\n", right -> name);
			if(right -> val == 148){
				insertRDec(tab,left->name,vh,
					   tp,scope ,left -> line,declare,"Grid");
				return;		   
			}
			for(i = 0 ; i< 200  && cnt < rightSize; i++){
				if(right-> child[i] != NULL){
					if(flag == 0){
						type  = right -> child[i] -> val;
						flag = 1;
					}
					else{
						if(type != right -> child[i] -> val){
							printf("Semantic Error: Array should have all values of same type\n");
							exit(0);
						}
					}
					strcat(vh, right->child[i]-> name);
					tp = right -> child[i] -> val;
					strcat(vh,",");
					cnt++;
				}
			}
			//TODO: handle all elements of array are same
			if(tp == 7)
				insertRDec(tab,left->name,vh,
					   tp,scope ,left -> line,declare,"Var");
			else
				insertRDec(tab,left->name,vh,
					   tp,scope ,left -> line,declare,"Arr");
		}
		else if(leftSize!=rightSize && leftSize > 0){
			printf("Semantic Error: \nLeft and Right hand size of Declaration not Compatible at line: '%d'\n",ast-> line);
			exit(0);	
		}
		
	}
	else{
		for(i= 0 ; i < 200 && cnt < 2; i++){
			if(ast -> child[i] != NULL && cnt == 0){
				left = ast->child[i];
				cnt++;
			}
			else if(ast ->child[i] != NULL && cnt == 1){
				right = ast->child[i];
				cnt++;
			}
		}
		// TODO: handle both sides of Assign Op //
		// Main Semantic Analysis code goes here //...
		leftSize = left -> numChild;
		rightSize = right -> numChild;
		//printf("....>>.......%s %d %s %d\n", left -> name , leftSize, right -> name, rightSize);
		if(leftSize != 0 && (leftSize != rightSize)){
			printf("Semantic Error: \nLeft and Right hand size of Declaration not Compatible at line: '%d'\n",ast-> line);
			exit(0);
		}
		else if(leftSize == 0){
			int res = lookUp(tab, left->name , scope);
			int res2 = lookUpPrev(tab,left->name,scope);
			int leftType;
			if( res == -1 && res2 == -1 ){
				printf("Semantic Error: \n'%s' is not Declared at line: '%d'\n",
					left->name,ast-> line);
				exit(0);				
			}
			else if(res != -1){
				updateRef(tab, res, ast->line);
				leftType = (tab->rec[res]).type;
			}
			else if(res == -1 && res2 != -1){
				updateRef(tab,res2,ast->line);
				leftType = (tab -> rec[res2]).type;
			}
			if(right -> val ==  20){
				//printf("%d\n", leftType);
				if(leftType != 24 && leftType != 23 ){
					printf("Semantic Error: Leftside is not of type String invalid operation at line no: %d\n",right -> line);
					exit(0);
				}
				int i,cnt = 0;
				pnode* leftT = NULL;
				pnode* rightT =  NULL;
				for(i = 0  ; i< 200 ; i++){
					if(right -> child[i] != NULL && cnt == 0){
						leftT = right-> child[i];
						cnt++;
					}
					else if(right-> child[i] != NULL && cnt == 1){
						rightT = right-> child[i];
						break;
					}
				}
				res = lookUp(tab, leftT->name , scope);
				res2 = lookUpPrev(tab,leftT->name,scope);
				if(res == -1 && res2 == -1){
					printf("Semantic Error: Variable '%s' not declared at line no: %d\n ", left->name, left->line );
					exit(0);
				}
				else{
					if(res != -1){

						if((tab -> rec[res]).type != 24){
							printf("Semantic Error: (.) operator is only valid with string but var of type %d \n",(tab -> rec[res]).type);
							exit(0);
						}
						updateReftoStr(tab, res, ast->line,rightT -> name);
					}
					else if(res2 != -1){
				
						if((tab -> rec[res2]).type != 24){
							printf("Semantic Error: (.) operator is only valid with string but var of type %d \n",(tab -> rec[res2]).type);
							exit(0);
						}
						updateReftoStr(tab,res2,ast->line, rightT -> name);
					}			
				}
				
				
			}
			// TODO: handle function call 
			else if(right -> val == 148){
				pnode* leftT =NULL;
				pnode* rightT = NULL;
				int i = 0,cnt = 0;
				for(i = 0 ; i< 200 ; i++){
					if(right -> child[i] != NULL && cnt == 0){
						leftT =  right  -> child[i];
						cnt ++;
					}
					else if(right -> child[i] != NULL && cnt == 1){
						rightT = right -> child[i];
						cnt++;
						break;
					}
				}
				if(rightT -> numChild == 0 || rightT-> val == 124){
				//	printf("%s\n", rightT -> name);
					return;
				}
				STable *func  =  lookUpFunc(sym,leftT -> name);
				int num_para =  rightT -> numChild;
				//printf("%d %d \n", num_para, func -> numPara);
				if(num_para !=  func -> numPara){
					printf("Semactic Error: No of parameters in function call and function '%s' decalration do match at line no: %d\n ",leftT-> name , leftT-> line);
					exit(0);
				}
				else{
					int i, j = 0;
					for(i = 0 ,j=0; i< num_para ; i++){
						if(rightT -> child[i] != NULL){
							if(rightT ->child[i] -> val == 7){
								int kk = lookUp(tab, rightT -> child[i] -> name, scope);
								int kk2  = lookUpPrev(tab, rightT -> child[i] -> name, scope);
								if(kk != -1){
									if( (func -> typePara[j] == 19 && (tab -> rec[kk]).type != 23) ||(func -> typePara[j] == 49 && (tab -> rec[kk]).type != 22)||(func -> typePara[j] == 31 && (tab -> rec[kk]).type != 24) ||(func -> typePara[j] == 30 && (tab -> rec[kk]).type != 50)  ){
										printf("Semantic Error: Type of parameters in function '%s' decalration and function call '%s' donot match at line no %d\n", leftT-> name , rightT -> child[i] -> name,leftT-> line);
									}	
								}
								else if(kk2 != -1){
									if( (func -> typePara[j] == 19 && (tab -> rec[kk2]).type != 23) ||(func -> typePara[j] == 49 && (tab -> rec[kk2]).type != 22)||(func -> typePara[j] == 31 && (tab -> rec[kk2]).type != 24) ||(func -> typePara[j] == 30 && (tab -> rec[kk2]).type != 50)  ){
										printf("Semantic Error: Type of parameters in function '%s' decalration and function call '%s' donot match at line no %d\n", leftT-> name , rightT -> child[i] -> name,leftT-> line);
									}	
								}
								else{
									j++;
								}
							}
							else if(rightT -> child[i] -> val != func -> typePara[j]){
								printf("Semantic Error: Type of parameters in function '%s' decalration and function call '%s' donot match at line no %d\n", leftT-> name , rightT -> child[i] -> name,leftT-> line);
								exit(0);
							}
							else{
								//printf("%d %d\n", rightT-> child[i] -> val, func-> typePara[j]);
								j++;
							}
						}
					}
				}
			}
			else if(right -> val == 32 || right-> val == 33 || right-> val == 34 || right -> val== 35){
				ValidateOpExp(right, leftType, tab,scope);
			}
			else if(right -> numChild == 0){
				if(right -> val ==  7){
					int res = lookUp(tab, right -> name , scope);
					int res2 = lookUpPrev(tab, right -> name , scope);
					if(res == -1 && res2 == -1){
						printf("Semantic Error: Variable '%s' is not declared at line no: %d\n",right -> name , right -> line);
						exit(0);
					}
					else if(res != -1){
						if((tab-> rec[res]). type != leftType){
							printf("Semantic Error: Variable '%s' is not of same type as left hand side at line no: %d\n",right -> name , right -> line);
							exit(0);
						}
					}
					else if(res2 != -1){
						if((tab-> rec[res2]). type != leftType){
							printf("Semantic Error: Variable '%s' is not of same type as left hand side at line no: %d\n",right -> name , right -> line);
							exit(0);
						}
					}
				}
				else if((right -> val == 19 && leftType != 23) ||(right -> val == 49 && leftType != 22)||(right -> val == 31 && leftType != 24) ||(right -> val == 30 && leftType != 50)  ){
					printf("Semantic Error: Variable '%s' is not of compatible type to left hand side at line no: %d\n",right->name,right->line);
					exit(0);
				}
			}
			
		}
	}
}
//===========================================================================//
void dfsHelper (STable* tab, pnode* ast, int scope, int declare,Master* sym){
	if(ast == NULL)
		return;
	int i,k;
	if(ast -> val ==103){
		int cnt = 0;
		for(i = 0  ; i < 200 && cnt < 4; i++){
			if((ast -> child[i] != NULL) && ast -> child[i] -> val == 7){
				strcpy(tab ->func_name,ast->child[i] -> name);	
			}
			else if( (ast -> child[i] != NULL) && ast -> child[i] ->val == 132){
				//printf("HERE\n");
				handleParList(tab,ast->child[i],scope);
				cnt++;
			}
			else if( (ast -> child[i] != NULL) && (ast -> child[i] -> val == 50 ||ast -> child[i] -> val == 21 || 
			ast -> child[i] -> val == 22|| ast -> child[i] -> val == 23 || 
			ast -> child[i] -> val == 24 || ast -> child[i] -> val == 49 )){
				tab -> returnType = ast -> child[i] -> val;
				cnt++;
			}
			else if(ast -> child[i] != NULL){
				//--> ON STATEMENT MARKER //
				if(ast -> val == 12){
					for(k = 0 ; k < 200 ; k++){
						if(ast -> child[k] != NULL){
							if(ast -> child[i] -> val == 13){
								declare++;
							}
							break;
						}
					}
					dfsHelper(tab,ast->child[i], scope, ++declare,sym);
					declare = 0;
				}
				else if(ast -> val == 45 || ast -> val == 25 || ast -> val == 27 || ast -> val == 26){
					dfsHelper(tab,ast->child[i], ++scope, declare,sym);
					--scope;
				}
				else 	
					dfsHelper(tab,ast->child[i], scope, declare,sym);
			}
		}
		
	}
	else if(ast -> val == 105 && ast -> par -> val == 101){
		//printf("HELLO\n");
		//printf("%d\n",ast -> par -> val);
		strcpy(tab ->func_name,"MAIN");
		tab -> returnType = -1;
		//--> ON STATEMENT MARKER //
		for(i = 0; i< 200 ; i++){
			if(ast->child[i] != NULL){
				if(ast -> val == 12){
					for(k = 0 ; k < 200 ; k++){
						if(ast -> child[k] != NULL){
							if(ast -> child[k] -> val == 13){
								declare++;
							}
							break;
						}
					}
					dfsHelper(tab,ast->child[i], scope, ++declare,sym);
					declare = 0;
				}
				else if(ast -> val == 45 || ast -> val == 25 || ast -> val == 27 || ast -> val == 26){
					dfsHelper(tab,ast->child[i], ++scope, declare,sym);
					--scope;
				}
				else
					dfsHelper(tab,ast->child[i], scope, declare,sym);
			}
		}
		
	}
	else if(ast -> val == 14){
		handleDeclarations(tab, ast,scope,declare,sym);
		return;
	}
	else if(ast -> val == 7){
		int res = lookUp(tab, ast -> name, scope);
		int temp = lookUpPrev(tab, ast -> name , scope);
		if((res != -1 && declare == 1) || (res == -1 && temp == -1 && declare == 0)){
			//printf("%d %d %d %s %d %d\n",res ,temp , declare,ast -> name,ast->line,scope);
			if(inKeyWords(ast -> name)){
				//insertRDec(tab,ast->name," ",-1,scope ,ast->line,0,"KeyWord");
				// Never enters this because DOT handled below else if
			}
			else{
				printf("Semantic Error: Variable '%s' not decalred on line no: %d\n",ast -> name ,ast->line);
				exit(0);
			}
		}
		else if(res !=-1 && declare == 0){
			updateRef(tab,res, ast -> line);
		}
		else if(res == -1 && temp != -1 && declare == 0){
			updateRef(tab,temp, ast -> line);
		}
		else{
			printf("%d %d %d %s %d %d\n",res ,temp , declare,ast -> name,ast->line,scope);
			insertR(tab, ast -> name ,scope, ast -> line);
		}
	}
	// TODO: Handle string ref //
	else if(ast-> val == 20){
		int i,cnt = 0;
		pnode* left = NULL;
		pnode* right =  NULL;
		for(i = 0  ; i< 200 ; i++){
			if(ast -> child[i] != NULL && cnt == 0){
				left = ast-> child[i];
				cnt++;
			}
			else if(ast-> child[i] != NULL && cnt == 1){
				right = ast-> child[i];
				break;
			}
		}
		int res = lookUp(tab, left->name , scope);
		int res2 = lookUpPrev(tab,left->name,scope);
		if(res == -1 && res2 == -1){
			printf("Semantic Error: Variable '%s' not declared at line no: %d\n ", left->name, left->line );
			exit(0);
		}
		else{
			//printf("HELL %d %d\n",res,res2);
			if(res != -1){
				
				if((tab -> rec[res]).type != 24){
					printf("Semantic Error: (.) operator is only valid with string but var of type %d \n",(tab -> rec[res]).type);
					exit(0);
				}
				updateReftoStr(tab, res, ast->line,right -> name);
			}
			else if(res2 != -1){
				
				if((tab -> rec[res2]).type != 24){
					printf("Semantic Error: (.) operator is only valid with string but var of type %d \n",(tab -> rec[res2]).type);
					exit(0);
				}
				updateReftoStr(tab,res2,ast->line, right -> name);
			}			
		}
		
	}
	//TODO: handle return statement
	else if(ast -> val == 9){
		//printf("Hello  return %s\n", tab -> func_name);
		int i = 0;
		pnode* left = NULL;
		for(i = 0 ; i < 200 ; i++){
			if(ast -> child[i] != NULL){
				left = ast -> child[i];
				break;
			}
		}
		//printf("%s\n", left -> name);		
		if(left -> val == 7) {
			int res = lookUp(tab,left -> name , scope);
			int res2  = lookUpPrev(tab,left -> name , scope);
			if(res== -1 && res2 == -1){
				printf(".Semantic Error: Variable '%s' not declared at line no %d\n", left -> name , left-> line);
				exit(0);
			}
			else if(res != -1){
				if((tab -> rec[res]).type != tab -> returnType ){
					printf("..Semantic Error: return type donot match with function return type at line no: %d\n", left -> line);
					exit(0);
				}
			}
			else if(res2 != -1){
				if((tab -> rec[res2]).type != tab -> returnType ){
					printf("...Semantic Error: return type donot match with function return type at line no: %d\n", left -> line);
					exit(0);
				}
			}
		}
		else if((left -> val == 19 && tab -> returnType != 23) || (left -> val == 49&& tab -> returnType != 22) || (left -> val == 31 && tab -> returnType != 24) || (left -> val == 30 && tab -> returnType != 50)){
			printf("....Semantic Error: return type donot match with function return type at line no: %d\n", left -> line);
			exit(0);	
		}
		
	}
	else{
		for(i = 0 ; i< 200 ; i++){
			if(ast -> val == 12){
				for(k = 0 ; k < 200 ; k++){
					if(ast -> child[k] != NULL){
						if(ast -> child[k] -> val == 13){
							declare++;
						}
						break;
					}
				}
				dfsHelper(tab,ast->child[i], scope, ++declare,sym);
				declare = 0;
			}
			else if(ast -> val == 45 || ast -> val == 25 || ast -> val == 27 || ast -> val == 26){
				dfsHelper(tab,ast->child[i], ++scope, declare,sym);
				--scope;
			}
			else
				dfsHelper(tab,ast->child[i], scope,declare,sym);
		}
	}
	
}
//=================================================================//
void dfsM(pnode* ast,Master* sym, int bound, int *count){
	if(ast == NULL)
		return;
	int i;
	if(*count == bound)
		return;
	if(ast-> val == 103 || ast -> val == 105){
		//printf("%d\n",*count);
		dfsHelper((*sym).func[(*count)++],ast,0,0,sym);
		//printf("FOUND\n");
		return;
	}
	for(i = 0 ; i< 200 ; i++){
		dfsM(ast -> child[i],sym,bound,count);
	}

}
//=================================================================//
void dfs_count(pnode* ast, int *count){
	int i;
	if(ast == NULL)
		return;
	if(ast-> val == 103 || ast -> val == 1){
		//printf("Eureka\n");
		(*count)++;
	}
	for(i = 0 ; i< 200 ; i++){
		dfs_count(ast->child[i], count);
	}
}
//=================================================================//
void printF(Master ta){
	FILE* fp = fopen("symbolTable.txt","w");
	STable* itr;
	int i = 0,j,k;
	fprintf(fp,"**********FUNCTION INFORMATION************\n",NULL);
	for(i =1 ; i <=ta.numFunc; i++){
		fprintf(fp,"Function %d:\n\tName:%s \t ReturnType: %d\n",i,(ta.func[i-1])->func_name,(ta.func[i-1])->returnType);
	}
	fprintf(fp,"*********SYMBOL TABLE **************\n",NULL);
	fprintf(fp,"FuncName\tVarName\t\tClass\tisMut\tScope\tRefCnt\tDecAt\tType\t  Value\t\tReferrencedAtLine\n");
	for(i = 0 ; i < ta.numFunc; i++){
		//printf("%s %d\n", (ta.func[i]) -> func_name , (ta.func[i]) -> returnType);
		itr = ta.func[i];
		for(j = 0 ; j < itr -> numRecords ; j ++){
			fprintf(fp,"%s%14s%18s%7d%8d%8d%8d%8d%12s\t\t", (itr -> rec[j]).func_name,(itr -> rec[j]).varName,(itr -> rec[j]).class,(itr -> rec[j]).isMutable, (itr -> rec[j]).scope, (itr -> rec[j]).refCnt,(itr -> rec[j]).lineNo,(itr -> rec[j]).type,(itr -> rec[j]).value );
			
			for(k = 0 ; k < (itr -> rec[j]).refCnt ; k++){
				fprintf(fp,"%d,",(itr -> rec[j]).refList[k]);
			}
			fprintf(fp,"\n",NULL);
		}
		//printf("%d\n", itr -> numRecords);
	}
	fclose(fp);
}
//=================================================================//
void checkDistinctAndMain(Master sym){
	int i,j;
	char str1[100];
	char str2[100];
	memset(str1,'\0',sizeof(str1));
	memset(str2,'\0',sizeof(str2));
	STable* itr;
	STable* itr2;
	int flag = 0;
	//printf("%d\n", sym.numFunc);
	
	for(i = 0 ; i< sym.numFunc ; i++){
		itr = sym.func[i];
		strcpy(str1, (itr) -> func_name);
		if(!strcmp(str1,"MAIN")){
			flag = 1;
		}
		//printf("%s\n", str1);
		for(j = 0 ; j < sym.numFunc ; j++){
			itr2 = sym.func[j];
			strcpy(str2, (itr2)->func_name);
			//printf("%s\n", str2);
			if(!strcmp(str1, str2) && i != j){
				printf("Semantic Error: Multiple declaration of same function name\n");
				exit(0);
			}
		}
	}
	if(flag == 0){
		printf("Semantic Error: No Main function in Program\n");
		exit(0);
	}
	
}

//=================================================================//
void getSymbolTable(Master *sym,pnode* ast){

	int i,count = 0;
	int noOfFunctions=0;
	dfs_count(ast, &noOfFunctions);
	(*sym).numFunc = noOfFunctions;
	
	
	//printf("No Of functions in the code: %d\n",noOfFunctions);
	for(i = 0 ; i< noOfFunctions ; i++){
		(*sym).func[i] = getNewTable();
	}
	dfsM(ast,sym,noOfFunctions,&count);
	checkDistinctAndMain(*sym);
	printF((*sym));
	return;
}
