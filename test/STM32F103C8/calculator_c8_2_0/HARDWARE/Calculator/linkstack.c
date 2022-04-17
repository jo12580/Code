#include "linkstack.h"



//SeqStack* Init_SeqStack()
//{
//	SeqStack* stack = (SeqStack*)malloc(sizeof(SeqStack));
//	for (int i = 0; i < MAX_SIZE; i++)
//	{
//		stack->data[i] = NULL;
//	}
//	stack->size = 0;
//	return stack;
//}

SeqStack_Symbol* Init_SeqStack_Symbol()
{
	int i = 0;
	SeqStack_Symbol* stack = (SeqStack_Symbol*)malloc(sizeof(SeqStack_Symbol));
	if(stack == NULL)
	{
		return NULL;
	}
	else
	{
		for (i = 0; i < MAX_SIZE; i++)
		{
			stack->data[i] = '\0';
		}
		stack->size = 0;
		return stack;
	}
}

SeqStack_Number* Init_SeqStack_Number()
{
	int i = 0;
	SeqStack_Number* stack = (SeqStack_Number*)malloc(sizeof(SeqStack_Number));
	if(stack == NULL)
	{
		return NULL;
	}
	else
	{
		for (i = 0; i < MAX_SIZE; i++)
		{
			stack->data[i] = 0.0;
		}
		stack->size = 0;
		return stack;
	}
}

void Push_SeqStack(SeqStack* stack, void* data)
{
	if (stack == NULL)
	{
		return;
	}
	if (stack->size >= MAX_SIZE)
	{
		return;
	}
	if (data == NULL)
	{
		return;
	}

	stack->data[stack->size] = data;
	stack->size++;

}

void Push_SeqStack_Symbol(SeqStack_Symbol* stack, char data)
{
	if (stack == NULL)
	{
		return;
	}
	if (stack->size >= MAX_SIZE)
	{
		return;
	}
	if (data == NULL)
	{
		return;
	}

	stack->data[stack->size] = data;
	stack->size++;
}

void Push_SeqStack_Number(SeqStack_Number* stack, double data)
{
	if (stack == NULL)
	{
		return;
	}
	if (stack->size >= MAX_SIZE)
	{
		return;
	}
	if (data == NULL)
	{
		return;
	}

	stack->data[stack->size] = data;
	stack->size++;
}

void* Top_SeqStack(SeqStack* stack)
{
	if (stack == NULL)
	{
		return NULL;
	}
	if (stack->size == 0)
	{
		return NULL;
	}

	return stack->data[stack->size - 1];
}

char Top_SeqStack_Symbol(SeqStack_Symbol* stack)
{
	if (stack == NULL)
	{
		return NULL;
	}
	if (stack->size == 0)
	{
		return NULL;
	}

	return stack->data[stack->size - 1];
}

double Top_SeqStack_Number(SeqStack_Number* stack)
{
	if (stack == NULL)
	{
		return NULL;
	}
	if (stack->size == 0)
	{
		return NULL;
	}

	return stack->data[stack->size - 1];
}

void Pop_SeqStack(SeqStack* stack)
{
	if (stack == NULL)
	{
		return;
	}
	if (stack->size == 0)
	{
		return;
	}
	stack->data[stack->size] = NULL;
	stack->size--;
}

void Pop_SeqStack_Symbol(SeqStack_Symbol* stack)
{
	if (stack == NULL)
	{
		return;
	}
	if (stack->size == 0)
	{
		return;
	}
	stack->data[stack->size] = NULL;
	stack->size--;
}

void Pop_SeqStack_Number(SeqStack_Number* stack)
{
	if (stack == NULL)
	{
		return;
	}
	if (stack->size == 0)
	{
		return;
	}
	stack->data[stack->size] = NULL;
	stack->size--;
}

int IsEmpty(SeqStack* stack)
{
	if (stack == NULL)
	{
		return -1;
	}
	if (stack->size == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsEmpty_Symbol(SeqStack_Symbol* stack)
{
	if (stack == NULL)
	{
		return -1;
	}
	if (stack->size == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsEmpty_Number(SeqStack_Number* stack)
{
	if (stack == NULL)
	{
		return -1;
	}
	if (stack->size == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	};
}

int Size_SeqStack(SeqStack* stack)
{
	return stack->size;
}

void Clear_SeqStack(SeqStack* stack)
{
	int i = 0;
	if (stack == NULL)
	{
		return;
	}
	for (i = 0; i < stack->size; i++)
	{
		stack->data[i] = NULL;
	}
	stack->size = 0;
}




void FreeSpace_SeqStack(SeqStack* stack)
{
	if (stack == NULL)
	{
		return;
	}

	free(stack);
}
