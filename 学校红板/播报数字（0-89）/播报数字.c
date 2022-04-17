  #include "reg52.h"	
  #include"stdio.h"		
typedef unsigned int u16;	 
typedef unsigned char u8;
typedef unsigned char uchar	  ;
typedef unsigned int  uint	;
sbit k1=P1^6;  //语音播放引脚定义 
sbit k2=P1^7;  //语音播放引脚定义 
sbit k3=P1^0;

sbit Led1=P1^4;
sbit Led2=P1^5;

sbit a1=P0^1; 
sbit a2=P0^2;
sbit a3=P0^3;
sbit a4=P0^4;
sbit a5=P0^5;
sbit a6=P0^6;
sbit a7=P0^7;
sbit a0=P0^0;

sbit b1=P1^0;
sbit b2=P1^1; 
sbit b3=P1^2;
sbit b4=P1^3;
sbit b5=P1^4;
sbit b6=P1^5;
sbit b7=P1^6;
sbit b8=P1^7;

sbit led=P2^0;
sbit rest=P2^7; //语音芯片复位引脚定义
sbit  dat=P2^6;  //语音芯片数据线引脚定义
sbit busy=P2^5;  //语音芯片---引脚定义

int j;

uchar a,b=1;
void music(uint i);
void  delay(uint i);

					void delay(u16 i)
						{
							while(i--);	
						}



void main()
{		
				if(k1==0)   
	  		  {				 
						 for(j=0;j<8;j++)

	{
if(j==0)  b1=1,b2=1,b3=1,b4=1,b5=1,b6=1,b7=1;		
if(j==1)  b1=0;
if(j==2)  b1=1,b2=0;
if(j==3)  b2=1,b3=0;
if(j==4)  b3=1,b4=0;
if(j==5)  b4=1,b5=0;
if(j==6)  b5=1,b6=0;
if(j==7)  b6=1,b7=0;
if(j==8)  b7=1,b8=0;

delay(50000);	//0	          
a1=1 ;
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=0 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);	 }
   else		music(12) ;
else 
            music(2);

delay(50000);//1
a1=1 ;
a2=1 ;
a3=1 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(3);}
   else    {music(12);while(!busy);music(3);  }
else      
           music(3);
		
delay(50000);//2
a1=0 ;
a2=2 ;
a3=0 ;
a4=0 ;
a5=1 ;
a6=0 ;
a7=0 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(4);}
   else    {music(12);while(!busy);music(4);  }
else      
           music(4);

		   
delay(50000);//3
a1=0 ;
a2=1 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=1 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(5);}
   else    {music(12);while(!busy);music(5);  }
else      
           music(5);
		
delay(50000);//4
a1=0 ;
a2=0 ;
a3=1 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(6);}
   else    {music(12);while(!busy);music(6);  }
else      
           music(6);

		
delay(50000);//5
a1=0 ;
a2=0 ;
a3=0 ;
a4=1 ;
a5=0 ;
a6=0 ;
a7=1 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(7);}
   else    {music(12);while(!busy);music(7);  }
else      
           music(7);

		   
delay(50000);//6
a1=0 ;
a2=0 ;
a3=0 ;
a4=1 ;
a5=0 ;
a6=0 ;
a7=0 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(8);}
   else    {music(12);while(!busy);music(8);  }
else      
           music(8);

		
delay(50000);//7
a1=1 ;
a2=1 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(9);}
   else    {music(12);while(!busy);music(9);  }
else      
           music(9);

		
delay(50000);//8
a1=0 ;
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=0 ;
a7=0 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(10);}
   else    {music(12);while(!busy);music(10);  }
else      
           music(10);

		
delay(50000);//9
a1=0 ;
a2=0 ;
a3=0 ;
a4=0 ;
a5=0 ;
a6=1 ;
a7=1 ;
if(j>0)
   if(j>1) {music(j+2);while(!busy);music(12);while(!busy);music(11);}
   else    {music(12);while(!busy);music(11);  }
else      
           music(11);
   
  
}   
   }
						
}  
		   
   				
				



void music(uchar i)							  //定义语音程序
{	
	dat=0;  
	rest=0;
	delay(50);
	rest=1;					
	delay(50);
	rest=0;
    delay(50);
	
	dat=0;
	delay(50);
	
	for(a=0;a<i;a++)
       {
		dat=1;
		delay(50);
		dat=0;
		delay(50);   
	   }
}


