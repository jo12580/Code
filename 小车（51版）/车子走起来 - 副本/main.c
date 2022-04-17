#include<reg52.h>
#include<drive.h>

void main()
{
	Timer0Init();
	while(1)
	{
		run();
	}	
}
	
	