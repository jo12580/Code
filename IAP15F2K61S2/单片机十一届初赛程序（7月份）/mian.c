/*******************************************************************************  
* 文件名称：蓝桥杯单片机第十一届省赛试题视频讲解对应程序（7月份）     
* 程序说明：如需了解有关本程序配套视频讲解，及比赛经验分享\
*					 欢迎访问淘宝店铺网址：shop117015787.taobao.com
* 日期版本：2021-03
*******************************************************************************/


#include<STC15F2K60S2.H>
#include<IIC.H>


uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,\
									0X40,0X79,0X24,0X30,0X19,0X12,0X02,0X78,0X00,0X10,\
									0XBF,0XFF,0XC1,0X8C,0XC8};//  "-"  "关"  U   P  n

unsigned char Digbuf[8] = {0,1,2,3,4,5,6,7};
uchar S1_Flag=0,S2_Flag=0,S3_Flag=0,S4_Flag=0;
uchar Digcom=0;
uchar Key_temp=0xff,key=0xff;

unsigned char AD=0;
unsigned int AD_Show=0;
unsigned char Mode=0;
unsigned char Dig_Show=0;
unsigned int Threshold=300;
unsigned int Count=0;

unsigned char AD_intr=0;
unsigned int Last_AD=0,Now_AD=0;
unsigned char LED_Bit=0XFF;
unsigned int AD_Compare_intr=0;
unsigned int Error_Key=0;

void delayms(int ms);
void allinit(void);
void keyscan(void);

void Timer2Init(void);
void DIG(void);

void main(void)
{
	allinit();
	Timer2Init();
//	EEPROM_write(0x00,30);delayms(5);
	Threshold=EEPROM_read(0X00)*10;delayms(5);
//	Digbuf[0]=Mode/10;Digbuf[1]=Mode%10;
	while(1)
	{
		if(AD_intr>=99)
		{
			AD_intr=0;
			AD=AD_read(0X03);
			Last_AD=Now_AD;
			Now_AD=AD*100/50;
			
			if((Now_AD<Threshold)&&(Last_AD>Threshold))
			{
				Count=Count+1;
			}
			
			if(Now_AD<Threshold){AD_Compare_intr++;}
			else {AD_Compare_intr=0;}
			
			if(AD_Compare_intr>=49){P2|=0X80;P2&=0X9F;LED_Bit&=0XFE; P0=LED_Bit;}
			else {P2|=0X80;P2&=0X9F;LED_Bit|=0X01; P0=LED_Bit;}
			
			if(Count%2==1){P2|=0X80;P2&=0X9F;LED_Bit&=0XFd; P0=LED_Bit;}
			else {P2|=0X80;P2&=0X9F;LED_Bit|=0X02; P0=LED_Bit;}
			
			if(Error_Key>=3){P2|=0X80;P2&=0X9F;LED_Bit&=0XFB; P0=LED_Bit;}
			else {P2|=0X80;P2&=0X9F;LED_Bit|=0X04; P0=LED_Bit;}
		
			DIG();
		}
		
		keyscan();
		
		//delayms(10);
	}
}

void keyscan(void)
{
	uchar i;
	uchar temp=0xff;

	for(i=0x80;i>8;i>>=1)										//1000 0000   0100 0000   = 0000 1000
	{
		temp=0xff;
		if(i==0x80) {P44=0;	P42=1;P3=(~i);}
		else if(i==0x40) {P42=0; P44=1;P3=(~i);}
		else { P42=1; P44=1; P3=(~i); }            
		
		if(i==0x80) {temp=P3;temp&=0x7f;}    // 0111 0111 
		else if(i==0x40) {temp=P3;temp&=0xbf;}// 1011 1111
		else temp=P3;
		
		if((temp&0x0f)!=0x0f)  //
		{
			switch(temp)
			{
				case 0x7e :Error_Key++;break;
				case 0x7d :Error_Key++;break;
				case 0x7b :Error_Key++;break;
				case 0x77 :Error_Key++;break;
				
				case 0xbe :Error_Key++;break;
				case 0xbd :Error_Key++;break;
				case 0xbb :Error_Key++;break;
				case 0xb7 :Error_Key++;break;
				
				case 0xde :Error_Key++;break;
				case 0xdd :Error_Key++;break;
				case 0xdb :
					if(Dig_Show==2){Count=0;}
					Error_Key=0;
				break;//13
				case 0xd7 :
					if(Dig_Show==0){Dig_Show=1;}
					else if(Dig_Show==1){Dig_Show=2;EEPROM_write(0x00,Threshold/10);}
					else if(Dig_Show==2){Dig_Show=0;}	
					Error_Key=0;
				break;//12
				
				case 0xee :Error_Key++;break;
				case 0xed :Error_Key++;break;
				case 0xeb :
					if(Dig_Show==1)
					{
						if(Threshold==0)Threshold=500;
						else Threshold=Threshold-50;
					}	
					Error_Key=0;
				break;//17
				case 0xe7 :
					if(Dig_Show==1)
					{
						if(Threshold==500)Threshold=0;
						else Threshold=Threshold+50;
					}
					Error_Key=0;
				break;//16	
				
				default : break;
			}		
		}
		
		while((temp&0x0f)!=0x0f)
		{
			temp=P3;
			temp=temp&0x0f;
		}		
	}
}

void DIG(void)
{
	if(Dig_Show==0)
	{
		AD_Show=AD*100/51;
		Digbuf[0]=22;Digbuf[1]=21;Digbuf[2]=21;Digbuf[3]=21;Digbuf[4]=21;
		Digbuf[5]=AD_Show/100+10;Digbuf[6]=AD_Show%100/10;Digbuf[7]=AD_Show%10;				
	}
	else if(Dig_Show==1)
	{
		Digbuf[0]=23;Digbuf[1]=21;Digbuf[2]=21;Digbuf[3]=21;Digbuf[4]=21;
		Digbuf[5]=Threshold/100+10;Digbuf[6]=Threshold%100/10;Digbuf[7]=Threshold%10;	
	}
	else if(Dig_Show==2)
	{
		if(Count<10){Digbuf[0]=24;Digbuf[1]=21;Digbuf[2]=21;Digbuf[3]=21;Digbuf[4]=21;Digbuf[5]=21;Digbuf[6]=21;Digbuf[7]=Count;}
		else if((Count>=10)&&(Count<100)){Digbuf[0]=24;Digbuf[1]=21;Digbuf[2]=21;Digbuf[3]=21;Digbuf[4]=21;Digbuf[5]=21;Digbuf[6]=Count/10;Digbuf[7]=Count%10;}
		else if((Count>=100)&&(Count<1000)){Digbuf[0]=24;Digbuf[1]=21;Digbuf[2]=21;Digbuf[3]=21;Digbuf[4]=21;Digbuf[5]=Count/100;Digbuf[6]=Count%100/10;Digbuf[7]=Count%10;}
		else if((Count>=1000)&&(Count<10000)){Digbuf[0]=24;Digbuf[1]=21;Digbuf[2]=21;Digbuf[3]=21;Digbuf[4]=Count/1000;Digbuf[5]=Count%1000/100;Digbuf[6]=Count%100/10;Digbuf[7]=Count%10;}
		else if((Count>=10000)&&(Count<100000)){Digbuf[0]=24;Digbuf[1]=21;Digbuf[2]=21;Digbuf[3]=Count/10000;Digbuf[4]=Count%10000/1000;Digbuf[5]=Count%1000/100;Digbuf[6]=Count%100/10;Digbuf[7]=Count%10;}
	}
}

void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;    //开定时器2中断
	EA=1;
}

//中断服务程序
void timer2int() interrupt 12           //中断入口
{
		P2|=0XC0;//打开位选573   U8
		P0=0XFF;  
		P2&=0XDF;
		P0=(1<<Digcom);
		P2|=0XE0;
		P0=0XFF;
		P2&=0XFF;//打开段选573   U7
		P0=tab[Digbuf[Digcom]];
		if(++Digcom==8)Digcom=0;
	
		AD_intr++;
}



void delayms(int ms)
{
	int i,j;
	for(i=ms;i>0;i--)
		for(j=845;j>0;j--);
}

void allinit(void)
{
	P2|=0XA0;
	P2&=0XBF;
	P0=0X00;//关闭蜂鸣器，继电器
	
	P2|=0X80;
	P2&=0X9F;
	P0=0XFF;//关闭LED灯
	
	P2|=0XC0;//打开位选573   U8
	P2&=0XDF;
	P0=0XFF;//选择所有数码管
	P2|=0XE0;//打开位选573   U7
	P2&=0XFF;
	P0=0XFF;//关闭所有数码管
}
