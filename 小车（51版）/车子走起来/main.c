#include<reg52.h>
#include<drive.h>

void main()
{
	Timer0Init();
	Timer1Init();
	while(1)
	{
		XunJi();
	}	
}
	
