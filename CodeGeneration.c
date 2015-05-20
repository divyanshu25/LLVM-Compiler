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

int lpcnt = 0;
int ifcnt = 0;

void genCode(pnode* ast,FILE* fp){
	if(ast == NULL)
		return;
	int i;
	pnode* temp;
	if(ast -> val == 12)
		return;
	if(ast -> val == 20){  // String structure
		int cnt = 0;
		pnode* left  =NULL;
		pnode* right = NULL;
		int i;
		for(i = 0 ; i< 200 ; i++){
			if(ast -> child[i] != NULL && cnt == 0){
				left = ast ->child[i];
				cnt++;
			}
			else if(ast -> child[i] != NULL && cnt == 1){
				right = ast -> child[i];
				break;
			}
		}
		//printf("%s %s \n", left -> name , right -> name);
		if(!strcmp(right -> name,"reverse")){
			//printf("HELLO\n");
			fprintf(fp,"\t mov esi ,%s_rev\n",left-> name);
			fprintf(fp,"\t mov edi ,%s\n",left-> name);
			fprintf(fp,"\t mov ecx ,%s_len\n",left-> name);
			fprintf(fp,"\tcld\n");
			fprintf(fp,"\trep	movsb\n");
		}
		if(!strcmp(right -> name,"toUpper")){
			//printf("HELLO\n");
			fprintf(fp,"\t mov esi ,%s_Upper\n",left-> name);
			fprintf(fp,"\t mov edi ,%s\n",left-> name);
			fprintf(fp,"\t mov ecx ,%s_len\n",left-> name);
			fprintf(fp,"\tcld\n");
			fprintf(fp,"\trep	movsb\n");
		}
		return;
	}		
	if(ast -> val ==  28){  // SCAN statements // 
		for(i = 0 ; i < 200 ; i++){
			if(ast -> child[i] != NULL){
				temp = ast -> child[i];
				break;
			}	
		}
		
		fprintf(fp,"\tmov eax,3\n"); // sys_read
		fprintf(fp,"\tmov ebx,2\n"); // std_in
		fprintf(fp,"\tmov ecx,%s\n",temp -> name);
		fprintf(fp,"\tmov edx,%s_len\n", temp->name);
		fprintf(fp,"\tint 80h\n\n");
		return;
	}
	if(ast -> val == 29){ // PRINT Statemets
		for(i = 0 ; i < 200 ; i++){
			if(ast -> child[i] != NULL){
				temp = ast -> child[i];
				break;
			}	
		}
		
		fprintf(fp,"\tmov eax,4\n"); // sys_write
		fprintf(fp,"\tmov ebx,1\n"); // std_out
		fprintf(fp,"\tmov ecx,%s\n",temp -> name);
		fprintf(fp,"\tmov edx,%s_len\n", temp -> name);
		fprintf(fp,"\tint 80h\n\n");
		
		fprintf(fp,"\tmov eax,4\n"); // sys_write
		fprintf(fp,"\tmov ebx,1\n"); // std_out
		fprintf(fp,"\tmov ecx,NL\n");
		fprintf(fp,"\tmov edx,1\n");
		fprintf(fp,"\tint 80h\n\n");
		return;
	}
	if(ast -> val == 25 || ast -> val == 26){ // IF statements
		int lfcnt = ifcnt++;
		pnode* cond =NULL;
		pnode* conds= NULL;
		pnode* condf = NULL;
		int cnt = 0;
		int i;
		for(i = 0; i< 200 ; i++){
			if(ast -> child[i] != NULL && cnt == 0){
				cond =  ast -> child[i];
				cnt ++;
			}
			else if(ast -> child[i] != NULL && cnt == 1){
				conds =  ast -> child[i];
				cnt ++;
			}
			else if(ast -> child[i] != NULL && cnt == 2){
				condf =  ast -> child[i];
				break;
			}
			
		}
		//printf("%s %s %s \n", cond-> name , conds -> name , condf -> name);
		pnode* left = NULL;
		pnode* right = NULL;
		cnt =0;
		for(i = 0 ; i< 200 ; i++){
			if(cond -> child[i] != NULL && cnt == 0){
				left = cond -> child[i];
				cnt++;
			}
			else if(cond-> child[i] != NULL && cnt == 1){
				right =  cond -> child[i];
				break;
			}
		}
		// OPTIMIZATIONS FOR IF CONDITION //
		
		if(left -> val != 7 && right -> val != 7 ){
			if(cond -> val == 40 && strcmp(left -> name , right -> name) > 0){
				genCode(conds,fp);
			}
			else if(cond -> val == 41 && strcmp(left -> name , right -> name) >= 0){
				genCode(conds,fp);
			}
			else if(cond -> val == 42 && strcmp(left -> name , right -> name) < 0){
				genCode(conds,fp);
			}
			else if(cond -> val == 43 && strcmp(left -> name , right -> name) <= 0){
				genCode(conds,fp);
			}	
			else if(cond -> val == 44 && strcmp(left -> name , right -> name) != 0){
				genCode(conds,fp);
			}
			else{
				genCode(condf,fp);
			}
			return;	
		}
		 
		
		
		if(left -> val == 7)
			fprintf(fp,"\tmov ecx,[%s]\n",left->name);
		else{
			fprintf(fp,"\tmov ecx,'%s'\n", left-> name);
			//fprintf(fp,"\tsub ecx,'0'\n");
		}
		if(right -> val == 7)
			fprintf(fp,"\tmov edx,[%s]\n",right->name);
		else{
			fprintf(fp,"\tmov edx,'%s'\n", right-> name);
			//fprintf(fp,"\tsub edx,'0'\n");
		}
		fprintf(fp,"\tcmp cl,dl\n");
		
		if(cond -> val == 39){
			fprintf(fp,"\tje IFLABEL_%d\n",lfcnt);
		}
		if(cond -> val == 40){
			fprintf(fp,"\tjg IFLABEL_%d\n",lfcnt);
		}
		if(cond -> val == 41){
			fprintf(fp,"\tjge IFLABEL_%d\n",lfcnt);
		}
		if(cond -> val == 42){
			fprintf(fp,"\tjl IFLABEL_%d\n",lfcnt);
		}
		if(cond -> val == 43){
			fprintf(fp,"\tjle IFLABEL_%d\n",lfcnt);
		}
		if(cond -> val == 44){
			fprintf(fp,"\tjne IFLABEL_%d\n",lfcnt);
		}
		genCode(condf,fp);
		fprintf(fp,"\t jmp EXITIF_%d\n",lfcnt);
		fprintf(fp,"IFLABEL_%d:\n",lfcnt);
		genCode(conds,fp);
		fprintf(fp,"EXITIF_%d:\n",lfcnt);
		return;	
	}
	if(ast -> val ==  45){  // LOOP statements
		pnode* left = NULL;
		pnode* right = NULL;
		int cnt= 0;
		int i;
		for(i = 0 ; i< 200 ; i++){
			if(ast -> child[i] != NULL && cnt == 0){
				left = ast -> child[i];
				cnt++;
			}
			else if(ast-> child[i] != NULL && cnt == 1){
				right =  ast -> child[i];
				break;
			}
		}	
		fprintf(fp,"Loop_%d:\n", lpcnt);
		genCode(right,fp);
		if(left -> val == 39 || left -> val == 40 ||left -> val == 41 ||left -> val == 42 ||left -> val == 43 || left-> val == 44){
			pnode* lt;
			pnode* rt;
			int c =0;
			for(i = 0 ; i< 200  ; i++){
				if(left -> child[i] != NULL && c ==0){
					lt = left -> child[i];
					c++;
				}
				else if(left -> child[i] != NULL && c == 1){
					rt = left -> child[i];
					break;
				}
			}
			fprintf(fp,"\tmov ecx , [%s]\n",lt -> name);
			//fprintf(fp,"\tmov edx , [%s]\n",rt -> name);
			if(rt -> val == 7)
				fprintf(fp,"\tcmp ecx,[%s]\n",rt->name);
			else{
				fprintf(fp,"\tsub ecx,'0'\n");
				fprintf(fp,"\tcmp ecx,%s\n",rt->name);
				
			}
			if(left -> val == 39)
				fprintf(fp,"\tje Loop_%d\n",lpcnt++);
			if(left -> val == 40)
				fprintf(fp,"\tjg Loop_%d\n",lpcnt++);
			if(left -> val == 41)
				fprintf(fp,"\tjge Loop_%d\n",lpcnt++);
			if(left -> val == 42)
				fprintf(fp,"\tjl Loop_%d\n",lpcnt++);
			if(left -> val == 43)
				fprintf(fp,"\tjle Loop_%d\n",lpcnt++);
			if(left -> val == 44)
				fprintf(fp,"\tjne Loop_%d\n",lpcnt++);
			
		}
		
		lpcnt++;
		return;
	}
	if(ast -> val == 14){  // assignment statement
			pnode* left = NULL;
			pnode* right = NULL;
			int cnt = 0,i;
			for(i = 0 ; i< 200 ;i++){
				if(ast -> child[i] != NULL && cnt == 0){
					left = ast -> child[i];
					cnt ++;
				}
				else if(ast -> child[i]!= NULL && cnt == 1){
					right =  ast -> child[i];
					break;
				}
			}
			//printf("%s %s\n", left -> name , right -> name);
			if(right -> numChild == 0 && right -> val != 7){
				fprintf(fp,"\tmov eax, '%s'\n", right -> name);	
			}
			else if(right -> numChild == 0 && right -> val == 7){
				//printf("%s %s\n", left -> name , right -> name);
				fprintf(fp,"\tmov eax, [%s]\n", right -> name);	
			}
			else if(right -> numChild == 2 && right -> val ==  148){
				pnode* nameArr = NULL;
				pnode* index = NULL;
				int cnt = 0,i;
				for(i =0 ; i < 200 ; i++){
					if(right -> child[i] != NULL && cnt == 0){
						nameArr = right-> child[i];
						cnt++;
					}
					else if(right -> child[i] != NULL && cnt == 1){
						index = right-> child[i];
						break;
					}
				}
				if(index -> val == 124){
					//printf("HELLO\n");
					int i;
					int cnt = 0;
					pnode* lindex =NULL;
					pnode* rindex = NULL;
					for(i =0 ; i < 200 ; i++){
						if(index -> child[i] != NULL && cnt == 0){
							lindex = index-> child[i];
							cnt++;
						}
						else if(index -> child[i] != NULL && cnt == 1){
							rindex = index-> child[i];
							break;
						}
					}
					//printf("%s %s\n", lindex -> name , rindex -> name);	
					fprintf(fp,"\tmov ecx,%s\n",nameArr -> name);
					fprintf(fp,"\t mov ebx,[%s]\n", lindex -> name);
					fprintf(fp,"\tsub ebx,'0'\n");
					fprintf(fp,"\t mov eax,3\n");
					fprintf(fp,"\t mul ebx\n");
					fprintf(fp,"\tadd eax,[%s]\n", rindex -> name);
					fprintf(fp,"\tsub eax,'0'\n");
					fprintf(fp,"\tmov edx,eax\n");
					fprintf(fp,"\tadd ecx,edx\n");
					fprintf(fp,"\tmov eax,[ecx]\n");
				}
				else{
					//printf("%s %s \n", nameArr -> name, index -> name);
					fprintf(fp,"\tmov ecx,%s\n",nameArr -> name);
					fprintf(fp,"\tmov edx,[%s]\n",index -> name);
					fprintf(fp,"\tsub edx,'0'\n");
					fprintf(fp,"\tadd ecx,edx\n");
					fprintf(fp,"\tmov eax,[ecx]\n");
					//fprintf(fp,"\tadd eax,'0'\n");
				}
				
			}
			else{
				
				if(right -> val == 33 || right -> val == 32 || right -> val == 34 || right -> val == 35){
					pnode* leftT;
					pnode* rightT;
					int cnt = 0;
					int i;
					for(i = 0 ; i< 200 ; i++){
						if(right -> child[i] != NULL && cnt == 0){
							leftT =  right -> child[i];
							cnt ++;
						}
						else if(right -> child[i] != NULL && cnt == 1){
							rightT = right -> child[i];
							break;
						}
					}
					
					if(leftT -> val ==  19){
						fprintf(fp,"\tmov [acc1],'%s'\n", leftT -> name);
					}
					else if(leftT -> val == 7){
						fprintf(fp,"\tmov eax,[%s]\n", leftT -> name);
						fprintf(fp,"\tmov [acc1],eax\n", leftT -> name);
					}
					if(rightT -> val ==  19){
						fprintf(fp,"\tmov eax,'%s'\n", rightT -> name);
					}
					else if(rightT -> val == 7){
						fprintf(fp,"\tmov eax,[%s]\n", rightT -> name);
						//fprintf(fp,"\tmov [acc2],eax\n", leftT -> name);
					}
					else if(rightT -> val == 33 || rightT -> val == 32 || rightT -> val == 34 || rightT -> val == 35 ){
						fprintf(fp,"\tmov ebx,[acc1]\n");
						fprintf(fp,"\tpush ebx\n");
						genCode(rightT,fp);
						fprintf(fp,"\tpop ebx\n");
						fprintf(fp,"\tmov [acc1],ebx\n");
					}
					fprintf(fp,"\tsub eax,'0'\n");
					if(right -> val == 33)
						fprintf(fp,"\tadd eax, [acc1]\n");
					else if(right ->val == 32){
						fprintf(fp,"\tmov ebx,[acc1]\n");
						fprintf(fp,"\tsub ebx,eax\n");
						fprintf(fp,"\tmov eax,ebx\n");
					}	
					else if(right -> val ==  35){
						fprintf(fp,"\tmov ebx,[acc1]\n");
						fprintf(fp,"\tsub ebx,'0'\n");
						fprintf(fp,"\tmul ebx\n");
						fprintf(fp,"\tadd eax,'0'\n");
					}
					else if(right -> val ==  34){
						fprintf(fp,"\tmov ebx,eax\n");
						fprintf(fp,"\tmov eax,[acc1]\n");
						fprintf(fp,"\tsub eax,'0'\n");
						fprintf(fp,"\tmov edx,0D\n");
						fprintf(fp,"\tdiv ebx\n");
						fprintf(fp,"\tadd eax,'0'\n");
					}									
				}
			}
			fprintf(fp,"\tmov [%s],eax\n", left -> name);
			return;
			
	}
	else if(ast -> val == 33 || ast -> val == 32 || ast -> val == 34 || ast -> val == 35 ){
		pnode* leftT = NULL;
		pnode* rightT = NULL;
		int cnt = 0,i;
		for(i = 0 ; i < 200 ; i++){
			if(ast -> child[i] != NULL && cnt == 0){
				leftT = ast -> child[i];
				cnt ++;
			}
			else if(ast -> child[i] != NULL && cnt == 1){
				rightT = ast -> child[i];
				break;
			}
		}
		if(leftT -> val ==  19){
			fprintf(fp,"\tmov [acc1],'%s'\n", leftT -> name);
		}
		else if(leftT -> val == 7){
			fprintf(fp,"\tmov eax,[%s]\n", leftT -> name);
			fprintf(fp,"\tmov [acc1],eax\n", leftT -> name);
		}
		if(rightT -> val ==  19){
			fprintf(fp,"\tmov eax,'%s'\n", rightT -> name);
		}
		else if(rightT -> val == 7){
			fprintf(fp,"\tmov eax,[%s]\n", rightT -> name);
			//fprintf(fp,"\tmov [acc2],eax\n", leftT -> name);
		}
		else if(rightT -> val == 33 || rightT -> val == 32 || rightT -> val == 34 || rightT -> val == 35){
			fprintf(fp,"\tmov ebx,[acc1]\n");
			fprintf(fp,"\tpush ebx\n");
			genCode(rightT,fp);
			
			fprintf(fp,"\tpop ebx\n");
			fprintf(fp,"\tmov [acc1],ebx\n");
		}
		fprintf(fp,"\tsub eax,'0'\n");
		if(ast -> val == 33)
			fprintf(fp,"\tadd eax, [acc1]\n");
		else if(ast ->val == 32){
			fprintf(fp,"\tmov ebx,[acc1]\n");
			fprintf(fp,"\tsub ebx,eax\n");
			fprintf(fp,"\tmov eax,ebx\n");
		}
		else if(ast -> val ==  35){
			fprintf(fp,"\tmov ebx,[acc1]\n");
			fprintf(fp,"\tsub ebx,'0'\n");
			fprintf(fp,"\tmul ebx\n");
			fprintf(fp,"\tadd eax,'0'\n");
		}	
		else if(ast -> val ==  34){
			fprintf(fp,"\tmov ebx,eax\n");
			fprintf(fp,"\tmov eax,[acc1]\n");
			fprintf(fp,"\tsub eax,'0'\n");
			fprintf(fp,"\tmov edx,0D\n");
			fprintf(fp,"\tdiv ebx\n");
			fprintf(fp,"\tadd eax,'0'\n");
		}	
		return;
	}
	for(i = 0 ; i< 200 ; i++){
		genCode(ast -> child[i] , fp);
	}
}


void GenerateCode(Master *sym,pnode* ast){
	FILE* fp;
	fp = fopen("sourceCode.asm","w");
	STable* itr;

	fprintf(fp,"section .data ;Data segment\n");
	
	
	itr = sym -> func[0];
	int j,i;
	for(j = 0 ; j < itr -> numRecords ; j ++){
		fprintf(fp,"\t");
		fprintf(fp,"%s ",(itr -> rec[j]).varName);
		if( ((itr-> rec[j]).type == 23 || (itr-> rec[j]).type == 50 || (itr-> rec[j]).type == 22 || (itr-> rec[j]).type == 7) && strcmp((itr-> rec[j]).class,"Arr")){
			fprintf(fp,"DD ");
			fprintf(fp,"%dD\n", (itr->rec[j]).value[0] + 0);
		}
		else if(!strcmp((itr-> rec[j]).class,"Arr")){
				//printf("%s\n", (itr->rec[j]).value); 
				fprintf(fp,"db ");
				int k;
				for(k = 0 ; k < (int)strlen((itr->rec[j]).value) ; k++){
					if((itr->rec[j]).value[k] != ',' && ((int)(itr->rec[j]).value[k]) != 39)
						fprintf(fp,"%d,", (int)(itr->rec[j]).value[k]);
				}
				fprintf(fp,"\n");
		}
		else if((itr-> rec[j]).type == 24){
			fprintf(fp,"db %s\n",((itr-> rec[j]).str) -> arr );
			fprintf(fp, "\t%s_len equ $-%s  ;$\n ",(itr -> rec[j]).varName,(itr -> rec[j]).varName);
			
			fprintf(fp,"\t%s_rev db %s\n",(itr -> rec[j]).varName,((itr-> rec[j]).str) -> rev_arr );
			//fprintf(fp, "\tlen%s_rev equ $-%s_rev  ;$\n ",(itr -> rec[j]).varName,(itr -> rec[j]).varName);
			
			fprintf(fp,"\t%s_Upper db %s\n",(itr -> rec[j]).varName,((itr-> rec[j]).str) -> Upper_arr );
			//fprintf(fp, "\tlen%s_upper equ $-%s_Upper  ;$\n ",(itr -> rec[j]).varName,(itr -> rec[j]).varName);
		}
		if((itr-> rec[j]).type == 23 ){
			fprintf(fp,"\t%s_len equ 1\n", (itr -> rec[j]).varName);
		}
		if((itr-> rec[j]).type == 50 ){
			fprintf(fp,"\t%s_len equ 1\n", (itr -> rec[j]).varName);
		}
	}
	
	fprintf(fp,"\ttemp DD 48D\n");
	fprintf(fp,"\tNL db 10D \n");
	fprintf(fp,"section .bss            ;Uninitialized data\n");
	
    fprintf(fp,"\tacc1 resb 4\n");
    fprintf(fp,"\tacc2 resb 4\n");
	fprintf(fp,"section .text\n");
    fprintf(fp,"\tglobal _start    ;must be declared for linker (gcc)\n");
	fprintf(fp,"_start:    ;tell linker entry point\n");
	genCode(ast,fp);
	fprintf(fp,"\tmov eax,1\n"); // sys_write
	fprintf(fp,"\tmov ebx,0\n"); // std_out
	fprintf(fp,"\tint 80h\n\n");
		
	
	fclose(fp);
return;
}
