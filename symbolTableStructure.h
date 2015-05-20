#ifndef SYMBOLTABLESTRUCTREH_
#define SYMBOLTABLESTRUCTREH_
//=========================================//
typedef struct string{
	char arr[2000];
	char rev_arr[2000];
	char Upper_arr[2000];
	int len;
	char name[100];
	int flagRev;
	int flagUpp;
}string;

//========================================//
typedef struct Grid{
	char value[2000];
	int xsize;
	int ysize;
	char name[100];
}grid;
//========================================//
typedef struct records{
	char func_name[100];
	char varName[100];
	char class[100];
	int refList[100];
	int refCnt;
	int lineNo;
	int type;
	char value[1000];
	int scope;
	int isMutable;
	int isStr;
	struct string* str;
} Records;

//=========================================//
typedef struct table{
	char func_name[100];
	int func_ID;
	int returnType;
	int numRecords;
	int numPara;
	int typePara[100];
	struct records rec[500];
} STable;

//========================================//
typedef struct master{
	int numFunc;
	struct table* func[10];
} Master;
//=========================================//



//=========================================//
#endif


