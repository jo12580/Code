#include<stdio.h>

int main()
{
int i,j,m;
for(i=1;i<=9;i++)
{
	for(j=1;j<=i;j++)
	{ 
		m=i*j;
		printf("%d * %d = %d     ",j,i,m);
	}
	printf("   \n");	
	}
//color("4");

}