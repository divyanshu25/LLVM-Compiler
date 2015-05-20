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
#include <string.h>
#include<stdlib.h>
#include<malloc.h>

int ind,input[10000],rules[200][20],parse_table[100][100],rule_count[200];
char terminals[100][100],nonTerminals[100][100],c,temp[100];
int count,input_size,in_pointer,st_pointer;
//==========================================================//
pnode* getNewNode(int idMapping, pnode* root){
	int i;
	pnode* ret= (pnode*) malloc(sizeof(pnode));
	ret -> isTerminal = 0;
	ret -> val = idMapping;
	ret -> deloc = 0;
	ret -> line = 0;
	if(idMapping > 100)
		strcpy(ret -> name, nonTerminals[idMapping-100]);
	else
		strcpy(ret -> name, terminals[idMapping]);
	ret->numChild=0;
	for(i=0;i<200;i++)
		ret->child[i]=NULL;
	ret->par=root;
	return ret;
}
//==========================================================//
pnode* DFS(pnode* root,char tokenValue[][100], int line[]){
	//printf("......input   %d\n",in_pointer);
	if(root == NULL)
		return root;

	int k,j;
	int val = root -> val;
	int input_top=input[in_pointer];
	int rule = -1;	
	if(val == 0){
		strcpy(root->name, "EPSILON");
		return root;
	}
	if(input_top == val && val < 100){
		root -> isTerminal = 1;
		root -> line = line[in_pointer];
		strcpy(root->name,tokenValue[in_pointer]);
		//printf("strike %d\n",val);
		in_pointer++;
		return root;
	}
	
	if(val<=100){
		printf("Parse Error at: '%s' at line no: %d\n",tokenValue[in_pointer],line[in_pointer]);
		exit(0);
	}

	rule=parse_table[val-100][input_top];
	//printf("%d %d inp=%d\n",input_top,val,in_pointer);

	if(rule<121){
		//printf("rule %d\n",rule);
		//printf("rule_count %d\n",rule_count[rule]);
		root->numChild=rule_count[rule];

		for(k=rule_count[rule],j=1;k>=1;k--,j++){
			//printf("%d %d\n",k,rules[rule][k]);
			root -> child[j-1] = getNewNode(rules[rule][k],root);
			//printf("%d ",root->child[j-1]->val);

		}

		//printf("\n");
		for(k =1; k <=rule_count[rule];k++){
			root -> child[k-1] = DFS(root -> child[k-1],tokenValue,line);
		}
	}

	else{
		printf("Parse Error at: '%s' at line no: %d\n",tokenValue[in_pointer],line[in_pointer]);
		exit(0);
	}

return root;
}

//==========================================================//

void inorder(pnode* root,FILE* fp,int tab){
	if(root==NULL)
		return;
	int i;
	for(i = 0 ; i < tab ; i++)
		fprintf(fp,"\t",NULL);
	
	fprintf(fp,"%s\n",root->name,root->val);
	/*if(root->isTerminal)
		fprintf(fp,"%s\n",root->name);
	else{
		fprintf(fp,"\n",NULL);
	}*/

	for(i=0;i<root->numChild;i++)
		inorder(root->child[i],fp,tab+1);

}

//==========================================================//
pnode* parse(char LexicalUnits[][100],char tokenValue[][100], int line[]){

	int i,j;
	FILE* fp=fopen("terminals.txt","r");
	FILE* fp2 = fopen("non terminals.txt","r");
	FILE* out = fopen("ParseTree.txt","w");
	FILE *rule_file,*parse_file,*debug;
	pnode* root=NULL;

	rule_file=fopen("grammar_final.txt","r");
	parse_file=fopen("ParseTable.txt","r");
	debug=fopen("debug.txt","w");

	while(fscanf(fp,"%s %d%c",temp,&ind,&c)!=EOF){
		strcpy(terminals[ind],temp);
	}
	//fclose(fp);
	while(fscanf(fp2,"%s %d%c",temp,&ind,&c)!=EOF){
		strcpy(nonTerminals[ind-100],temp);
	}
	fclose(fp2);
	for(i=0;LexicalUnits[i][0]!='\0';i++){
		for(j=1;j<=50;j++){
			if(strcmp(LexicalUnits[i],terminals[j])==0){
				input[i]=j;
				//printf(" %d %s %s\n ",input[i],tokenValue[i],LexicalUnits[i]);
				break;
			}
		}
		//printf("%s %s %s\n",LexicalUnits[i],tokenValue[i],terminals[j]);
	}
	//printf("\n");
	input_size=i;

	for(i=1;i<=120;i++){

		fscanf(rule_file,"%d",&count);
		rule_count[i]=count;
		for(j=count;j>=1;j--){
			fscanf(rule_file,"%d",&rules[i][j]);
		}
	}

	for(i=1;i<=59;i++){
		for(j=1;j<=50;j++){
			fscanf(parse_file,"%d",&parse_table[i][j]);
		}
	}
	in_pointer=0;
	root = getNewNode(101,NULL);
	root = DFS(root,tokenValue,line);
	inorder(root,out,0);
	return root;
	fclose(fp);
	fclose(rule_file);
	fclose(parse_file);
	fclose(fp2);
	fclose(out);
	
}  
