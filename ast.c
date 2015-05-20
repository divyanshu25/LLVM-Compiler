//==========================================================//
//		PROJECT: Compiler Construction						//						
//		Group Number : 15                                   //
//		Team Number : 1										//
//		Team Members:										//
//			1. Divyanshu Goyal		2011B4A7795P			//
//			2. Abhishek Modi		2011B4A7331P			//
//															//
//==========================================================//

#include "parseStructure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int list[]={2,3,4,5,6,10,15,16,17,18,27,48},size=12;
int optList[] = {9,20,14,25,26,28,29,32,33,34,35,36,37,38,39,40,41,42,43,44,45,12},size2 = 22;
int shiftList[]= {20,32,33,34,35,36,37,38,39},size3 = 9;
//========================================//
int inList(int val){
	int i;
	for(i=0;i<size;i++)
		if(val==list[i])
			return 1;

	return 0;
}
//=========================================//
int inOptList(int val){
	int i;
	for(i=0;i<size2;i++)
		if(val==optList[i])
			return 1;

	return 0;
}
//=========================================//
int inShiftList(int val){
	int i;
	for(i=0;i<size3;i++)
		if(val==shiftList[i])
			return 1;

	return 0;
}
//=========================================================//
pnode* getASTHelper(pnode* root){
	if(root == NULL)
		return root;
	
	int i,child;
	pnode* ret = NULL;

	if(root->numChild != 0){
		child = root->numChild;
		for(i=0;i<child;i++){
			if(root->child[i]!=NULL)
				root->child[i]=getASTHelper(root->child[i]);
		}
	}	

	if(root->val==0 || inList(root->val)){
		(root-> par-> numChild)--;
		free(root);
		return NULL;
	}

	if(root->numChild==0 && root->isTerminal!=1){
		(root-> par-> numChild)--;
		free(root);
		return NULL;
	}

	if(root->numChild==1){
		for(i=0;i<200;i++){
			if(root->child[i]!=NULL){
				ret=root->child[i];
				root->child[i]->par=root->par;
				root->par = NULL;
				free(root);
				return ret;
			}
		}
	}
		return root;
}
//===========================================================//
pnode* MergeOptimization(pnode* root){
	int i,k,j;
	if(root == NULL)
		return NULL;
	for(i = 0 ; i< 200 ; i++){
		if(root->child[i] != NULL)
				root-> child[i] = MergeOptimization(root->child[i]);
	}
	for(i = 0 ; i< 200 ;  i++){
		if(root -> child[i] !=  NULL &&  ((root -> child[i] -> val == root-> val && root -> val > 100) ||((root -> child[i] -> val == 106 && root -> val == 105)) )){
			int ptr;
			for(j = 0 ; j < 200 ; j++){
				int flag  = 0;
				for(k = j ; k < 200 ; k++){
					if(root -> child[k] != NULL){
						flag = 1;
						break;
					}
				}
				if(flag == 1)
					continue;
				else{
					ptr = j;
					break;
				}
			}
			for(j = 0 ; j< 200; j++){
				if(root->child[i]->child[j] != NULL){
					root->child[ptr++] = root->child[i]->child[j];	
				}
						
			}
			root->numChild += root->child[i]->numChild;
			free(root->child[i]);
			root->child[i] = NULL;
			(root->numChild)--;
			break;
		}
	}
	return root;

}
//==========================================================================//
pnode* optimizeAst(pnode* root){
	int i,k,j;
	if(root == NULL)
		return NULL;
	for(i = 0 ; i< 200 ; i++){
		if(root->child[i] != NULL)
				root-> child[i] = optimizeAst(root->child[i]);
	}
	int count = 0,count2 = 0;
	if((root -> numChild) > 1){
		for(i = 0 ; i< 200 ; i++){
			if((root->child[i]!= NULL) && inOptList(root->child[i] -> val) && (root->child[i] ->deloc == 0) ){
				count++;
			}
			if((root->child[i]!= NULL) && (inOptList(root->child[i] -> val)) && (root->child[i] ->deloc == 1) ){
				count2++;
			}
		}
		if(count == 1){
			for(i =0 ; i< 200 ; i++){
				if((root->child[i]!= NULL) && inOptList(root->child[i] -> val)){
					
					root-> val = root->child[i]-> val;
					root->line = root->child[i] ->line;
					strcpy(root->name, root->child[i]->name);
					root->deloc = 1;
					free(root->child[i]);
					root->child[i] = NULL;
					(root->numChild)--;
					break;
				}
			}
		}
		else if(count == 0 &&count2 == 1){
			for(i =0 ; i< 200 ; i++){
				if((root->child[i]!= NULL) && (inShiftList(root->child[i] -> val) || (root->child[i] -> val == 14 && root -> val == 107))){
					//printf("%d %d\n", root -> child[i] -> line, root -> child[i] -> val);
					root-> val = root->child[i]-> val;
					strcpy(root->name, root->child[i]->name);
					root -> line = root->child[i] ->line;
					root->deloc = 1;
					int ptr;
					for(j = 0 ; j < 200 ; j++){
						int flag  = 0;
						for(k = j ; k < 200 ; k++){
							if(root -> child[k] != NULL){
								flag = 1;
								break;
							}
						}
						if(flag == 1)
							continue;
						else{
							ptr = j;
							break;
						}
					}
					for(j = 0 ; j< 200; j++){
						if(root->child[i]->child[j] != NULL){
							root->child[ptr++] = root->child[i]->child[j];	
						}
								
					}
					root->numChild += root->child[i]->numChild;
					free(root->child[i]);
					root->child[i] = NULL;
					(root->numChild)--;
					break;
				}
			}

		}


	}
	return root;

}
//===========================================================//

void inorderAST(pnode* root,FILE* fp,int tab){

	if(root==NULL)
		return;
	int i;
	for(i = 0 ; i < tab ; i++)
		fprintf(fp,"\t",NULL);
	
	fprintf(fp,"%s L:%d C:%d V:%d \n",root->name,root->line,root->numChild, root -> val, root -> deloc);

	for(i=0;i<200;i++)
		inorderAST(root->child[i],fp,tab+1);

}

//=========================================================//

pnode* getAST (pnode* root){
	int i,child;

	FILE* fp=fopen("AbstractSyntaxTree.txt","w");
	
	//child=root->numChild;

	//for(i=0;i<child;i++)
		//root->child[i]=getASTHelper (root->child[i]);
	root = getASTHelper(root);
	root = optimizeAst(root);
	root = MergeOptimization(root);
	inorderAST(root,fp,0);
	fclose(fp);
	return root;
}
