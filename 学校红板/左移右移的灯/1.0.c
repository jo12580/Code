#include<reg52.h>
int i;
void delay(int a)
{
while(a--);
}

void main()
{
while(1)
	{
	for(i=0;i<=7;i++)
		P2=(0x01<<i),delay(50000);
	for(i=0;i<=7;i++)
		P2=(0x80>>i),delay(50000);
	}
}