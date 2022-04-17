#ifndef _LINKSTACK_H
#define _LINKSTACK_H

#include "stdlib.h"


//数组模拟栈的顺序存储
#define MAX_SIZE 40
#define SEQSTACK_TRUE 1
#define SEQSTACK_FLASE 0


//栈顶为数组底部右边
typedef struct SEQSTACK {
	void * data[MAX_SIZE];
	int size;
}SeqStack;

typedef struct SEQSTACKSYMOBL {
	char data[MAX_SIZE];
	int size;
}SeqStack_Symbol;

typedef struct SEQSTACKNUMBER {
	double data[MAX_SIZE];
	int size;
}SeqStack_Number;

//初始化栈
SeqStack* Init_SeqStack();
SeqStack_Symbol* Init_SeqStack_Symbol();
SeqStack_Number* Init_SeqStack_Number();
//入栈
void Push_SeqStack(SeqStack *stack, void* data);
void Push_SeqStack_Symbol(SeqStack_Symbol* stack, char data);
void Push_SeqStack_Number(SeqStack_Number* stack, double data);
//返回栈顶元素
void* Top_SeqStack(SeqStack* stack);
char Top_SeqStack_Symbol(SeqStack_Symbol* stack);
double Top_SeqStack_Number(SeqStack_Number* stack);
//出栈
void Pop_SeqStack(SeqStack* stack);
void Pop_SeqStack_Symbol(SeqStack_Symbol* stack);
void Pop_SeqStack_Number(SeqStack_Number* stack);
//是否为空
int IsEmpty(SeqStack* stack);
int IsEmpty_Symbol(SeqStack_Symbol* stack);
int IsEmpty_Number(SeqStack_Number* stack);
//返回栈中的元素个数
int Size_SeqStack(SeqStack *stack);
//清空
void Clear_SeqStack(SeqStack* stack);
//销毁
void FreeSpace_SeqStack(SeqStack* stack);



#endif

