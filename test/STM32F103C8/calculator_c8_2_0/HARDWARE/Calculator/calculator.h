#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include "linkstack.h"
#include "stdlib.h"
#include "math.h"


//清空数组的内容
void clearArray(char* Array);
//通过运算符计算结果
double calculate(double a,double b,char operators);
//中缀表达式转后缀表达式  且  计算出后缀表达式的结果
double calculator(char * expression);


#endif