#include<stdio.h>

								   //	 ÉýÐò
int main()
{
	int a[7]={2,5,4,1,2,3,6};
	int i,j,b;
	for(i=0;i<=6;i++)
	{
		for(j=0;j<=i-j;j++)
		{
			if(a[j]>a[j+1])
			{
				b=a[j];
				a[j]=a[j+1];
				a[j+1]=b;
			}
		}
	}
	for(i=0;i<=6;i++)
	printf("%d \n",a[i]);
}