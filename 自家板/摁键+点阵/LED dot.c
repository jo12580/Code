#include<reg52.h>

void Hc595SendByte(u8 dat)
{											   
	u8 a;
	SRCLK=0;
	RCLK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RCLK=1;
	_nop_();					   //ясЁы
	_nop_();
	RCLK=0;
}