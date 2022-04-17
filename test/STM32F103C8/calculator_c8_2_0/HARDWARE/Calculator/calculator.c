#include "calculator.h"


/*
1.凡是遇到数字直接压入数据栈。
2.
a、当遇到运算符（加、减、乘、除），先判断算符栈中是否为空：
若为空：二话不说直接把它压入算符栈。
若不为空：比较运算符优先级之后，判断是否压入算符栈；
弹出符号是一直弹出到符号条件为之
b、 当遇到运算符（右括号），直接压入算符栈，因为左括号的优先级比加减乘除都高。
c、当遇到运算符（左括号），直接取出算符栈的运算符，直到匹配到左括号为止。
d、当遇到标志符（#），如果是第一个：直接压入算符栈，因为第一个#是标志的算式的开始，它必须进入算符栈；如果是最后一个，直接取出算符栈中的符号，直到匹配到 # 为止。

根号的特殊处理 根号7  这种无视上面的关系先入栈，然后遇到下个运算符，出栈数字进行计算
				4根号7设置根号处理标志, 根号不算在运算符中
				 r 作为根号实部与虚部的标志 
平方优先级 大于 * / 
*/
//清空里面存的数据



void clearArray(char* Array)
{
	unsigned char i;
	for ( i = 0; i < sizeof(Array) / sizeof(char) + 1; i++)
	{
		Array[i] = '\0';
	}
}

double calculate(double a,double b,char operators)
{
	double tempval = 0.0;
	switch (operators)
	{
	case '+':tempval = a + b;
		break;
	case '-':tempval = a - b;
		break;
	case '*':tempval = a * b;
		break;
	case '/':tempval = a / b;
		break;
	case '^':tempval = pow(a,b);
		break;
	default:
		break;
	}
	return tempval;
}


double calculator(char * expression)
{
	int i = 0;
	int j = 0;
	unsigned char sqrt_flag = 0;//根号标志
	char tempNumberArray[10] = "\0\0\0\0\0\0\0\0\0";//用于临时存入数字
	double tempNumber_t;//用于临时存入数字变成  计算机数字的中间变量
	char Top_element;//栈顶元素
	double tempright;//右操作数
	double templeft;//左操作数
	double mid_val;//用于后缀表达式计算中间的临时结果
	SeqStack_Number* NumberStack = Init_SeqStack_Number();//数字栈
	SeqStack_Symbol* SymbolStack = Init_SeqStack_Symbol();;//符号栈
	
	//遍历运算表达式
	while (expression[i] != '\0')
	{
		//遇到#号加入符号栈
		if (expression[i] == '#')
		{
			
			//如果是第一个直接入栈，最后一个要转换最后一个数字
			if (i != 0)
			{
				j = 0;
				Push_SeqStack_Symbol(SymbolStack, expression[i]);
				if (sqrt_flag == 0)
				{
					tempNumber_t = atof(tempNumberArray);
					Push_SeqStack_Number(NumberStack, tempNumber_t);
				}
				else
				{
					sqrt_flag = 0;
					//说明有根号需要处理
					//处理虚部
					tempNumber_t = atof(tempNumberArray);
					tempNumber_t = sqrt(tempNumber_t);
					//将计算结果入栈
					Push_SeqStack_Number(NumberStack, tempNumber_t);
				}
			}
			else
			{
				Push_SeqStack_Symbol(SymbolStack, '#');
			}

		}
		//遇到算数符号加入符号栈
		else if (expression[i] == '+' || expression[i] == '*' || expression[i] == '/' || expression[i] == '-' || expression[i] == '^')
		{
			//说明上个数字已经完成把数字转换成double数据存入数字栈
			if (sqrt_flag == 0)
			{
				tempNumber_t = atof(tempNumberArray);
				Push_SeqStack_Number(NumberStack, tempNumber_t);
			}
			else
			{
				sqrt_flag = 0;
				//说明有根号需要处理
				//处理虚部
				tempNumber_t = atof(tempNumberArray);
				tempNumber_t = sqrt(tempNumber_t);
				//将计算结果入栈
				Push_SeqStack_Number(NumberStack, tempNumber_t);
			}
			
			j = 0;
			clearArray(tempNumberArray);
			//先判断符号栈是否为空，即栈顶元素是否为 '#'
		  Top_element = Top_SeqStack_Symbol(SymbolStack);
			if (Top_element == '#')
			{
				//直接入栈
				Push_SeqStack_Symbol(SymbolStack, expression[i]);
			}
			//优先级最高的入栈
			else if (expression[i] == '^')
			{
				Push_SeqStack_Symbol(SymbolStack, expression[i]);
			}
			//如果是* /需要判断栈顶优先级
			else if (expression[i] == '*' || expression[i] == '/')
			{
				if ( Top_element == '^')
				{
					while (Top_element == '^')
					{
						//先出栈顶元素符号
						Pop_SeqStack_Symbol(SymbolStack);
						//然后出栈两个操作数
						tempright = Top_SeqStack_Number(NumberStack);
						Pop_SeqStack_Number(NumberStack);
						templeft = Top_SeqStack_Number(NumberStack);
						Pop_SeqStack_Number(NumberStack);
						//进行计算
						mid_val = calculate(templeft, tempright, Top_element);
						//将计算结果入栈
						Push_SeqStack_Number(NumberStack, mid_val);
						Top_element = Top_SeqStack_Symbol(SymbolStack);
					}
					//入栈运算符
					Push_SeqStack_Symbol(SymbolStack, expression[i]);
				}
				else
				{
					Push_SeqStack_Symbol(SymbolStack, expression[i]);
				}
			}
			else if(expression[i] == '+' || expression[i] =='-')
			{
				if (Top_element == '*' || Top_element == '/' || Top_element == '^')
				{
					while (Top_element == '*' || Top_element == '/' || Top_element == '^')
					{
						//先出栈顶元素符号
						Pop_SeqStack_Symbol(SymbolStack);
						//然后出栈两个操作数
						tempright = Top_SeqStack_Number(NumberStack);
						Pop_SeqStack_Number(NumberStack);
						templeft = Top_SeqStack_Number(NumberStack);
						Pop_SeqStack_Number(NumberStack);
						//进行计算
						mid_val = calculate(templeft, tempright, Top_element);
						//将计算结果入栈
						Push_SeqStack_Number(NumberStack, mid_val);
						Top_element = Top_SeqStack_Symbol(SymbolStack);
					}
					//入栈运算符
					Push_SeqStack_Symbol(SymbolStack, expression[i]);
				}
				else
				{
					Push_SeqStack_Symbol(SymbolStack, expression[i]);
				}
			}
		}
		//判断是否为根号
		else if (expression[i] == 'r')
		{
			//标志改变
			sqrt_flag = 1;
			//添加一个根号虚实实部标志
		}
		else if((expression[i] >= '0') && (expression[i] <= '9'))
		{
			tempNumberArray[j] = expression[i];
			j++;
		}


		i++;
	}

	//遍历完成将符号栈的元素弹出计算
	while (!IsEmpty_Symbol(SymbolStack))
	{
		//弹出栈顶符号
		Top_element =  Top_SeqStack_Symbol(SymbolStack);
		if (Top_element != '#')
		{

			//弹出两个操作数
			tempright = Top_SeqStack_Number(NumberStack);
			Pop_SeqStack_Number(NumberStack);
			templeft = Top_SeqStack_Number(NumberStack);
			Pop_SeqStack_Number(NumberStack);
			mid_val = calculate(templeft, tempright, Top_element);
			//结果入栈
			Push_SeqStack_Number(NumberStack, mid_val);
			//符号出栈
			Pop_SeqStack_Symbol(SymbolStack);
		}
		else
		{
			//直接出栈#
			Pop_SeqStack_Symbol(SymbolStack);
		}
	}
	free(NumberStack);
	free(SymbolStack);//释放内存
	return Top_SeqStack_Number(NumberStack);
	
}




