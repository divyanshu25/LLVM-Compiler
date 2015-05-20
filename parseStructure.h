#ifndef PARSESTRUCTREH_
#define PARSESTRUCTREH_

typedef struct nd{

	int val;
	int isTerminal;
	int numChild;
	struct nd* child[200];
	struct nd* par;
	char name[100];
	int deloc;
	int line;
} pnode;

#endif
