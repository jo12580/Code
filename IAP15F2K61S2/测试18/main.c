#include<stc15f2k60s2.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
	
sbit DQ=P1^4;

uchar tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,\
						 0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10,0XBF,0XFF};
uchar num;
long Wendu;

uchar DisplayData[9];
void Delayms(int ms);
void Delay500us();
void Delay100us();
void Delay80us();
//void Init_DS18b20(void);
bit Init_DS18b20(void);
void DS18B20_WiteByte(unsigned char dat);
long TemperGet(void);
void datapros();
void Display();
						 
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
	P2=0XA0;
	P0=0X00;
	while(1)
	{
		Display();
		if((Wendu%100000/10000)==8)
		{
			P2=0XA0;
			P0=0xff;
			P2=0x80;
			P0=0xff;
			Delay100ms();
			P2=0XA0;
			P0=0x00;
			P2=0x80;
			P0=0x00;
		}
	}
}

void Display()
{
	unsigned int i;
	datapros();
	for(i=0;i<=5;i++)
	{
		P2=0xc0;
    switch(i)
		{
			case 0:P0=0x01;break;
			case 1:P0=0x02;break;
			case 2:P0=0x04;break;
			case 3:P0=0x08;break;
			case 4:P0=0x10;break;
			case 5:P0=0x20;break;
			case 6:P0=0x40;break;
			case 7:P0=0x80;break;
		}
		P2=0xe0;
		P0= DisplayData[i];
		Delayms(1);	
		P0=0xff;	
	}
}

void datapros() 	
{
  Wendu=TemperGet();
	DisplayData[5] = tab[Wendu%10];				
	DisplayData[4] = tab[Wendu%100/10];				 
	DisplayData[3] = tab[Wendu%1000/100];	
	DisplayData[2] = tab[Wendu%10000/1000];					
	DisplayData[1] = tab[Wendu%100000/10000+10];		
	DisplayData[0] = tab[Wendu/100000];	
	DisplayData[6] = tab[1];			
	DisplayData[7] = tab[1];	
}
						
bit Init_DS18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;

	Delay500us();
	  	DQ = 0;

	Delay500us();
	  	DQ = 1;
	Delay500us();
	    initflag = DQ; 
		Delay500us();
	  	return initflag;

}

void DS18B20_WiteByte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ=0;
		DQ=dat&0x01;//1100 1100 & 0000 0001 = 0000 0000
		Delay80us();
		DQ=1;
		dat >>=1; //  0110 0110  //0011 0011 // 0000 0000
 	}
}

unsigned char DS18B20_ReadByte(void)
{
	unsigned char i;
	unsigned char dat;//0000 0000 
	
	for(i=0;i<8;i++)
	{
		DQ=0;
		dat >>=1;					 // 0101 0000 
		DQ=1;
		if(DQ==1)
		{
			dat=dat|0x80;  // 0000 0000 | 1000 0000 = 1000 0000
		}
		Delay80us();
	}
	
	return dat;
}

long TemperGet(void)
{
	unsigned char h,l;
	long temp;
	Init_DS18b20();
	DS18B20_WiteByte(0xcc);
	DS18B20_WiteByte(0x44);
	Delay500us();
	Init_DS18b20();
	DS18B20_WiteByte(0xcc);
	DS18B20_WiteByte(0xbe);
	l=DS18B20_ReadByte();
	h=DS18B20_ReadByte();
	temp=h<<8;
	temp|=l;
	temp=temp*625;
	return temp;
}

void Delay80us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 1;
	j = 217;
	do
	{
		while (--j);
	} while (--i);
}

//void Delay100us()		//@11.0592MHz
//{
//	unsigned char i, j;

//	_nop_();
//	_nop_();
//	i = 2;
//	j = 15;
//	do
//	{
//		while (--j);
//	} while (--i);
//}

void Delay500us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 6;
	j = 93;
	do
	{
		while (--j);
	} while (--i);
}

void Delayms(int ms)
{
	int i,j;
	for(i=0;i<ms;i++)
		for(j=845;j>0;j--);
}
