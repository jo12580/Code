#include<reg52.h>
#include <74hc595.h>
#include <IIC-1602.h>
#include <HX711.h>
#include <jianpan.h>
#include<stdio.h>

sbit LED0=P0^0;  //

void  delay(unsigned int i);
unsigned long HX711_Buffer;   //  
		bit firstflag;    // 	

		unsigned char count=20;//1次中断时间约为20ms，50次T1中断为1秒

//============== 
void  Time1Iinitial()
{
	TMOD=0x10;  	 	//T1方式1
   TH1 =0x00;   		//初值
   TL1=0x00;  
   EA = 1;   		//
   ET1=1;   		// 允许T1中断
//   TR1=1; 		//启动T1 
}


//==============

void Timer1_ISR (void) interrupt 3 
{

   TH1=0x00;    		//重装初值
   TL1=0x00;
   count--;     	 	//
   if (count==0) //
   {
     count=20;
     firstflag=0;
   }
}


void main()
{   
	  unsigned char s; // 
    unsigned char  Data_sw,Data_w,Data_q,Data_b,Data_s,Data_g; // 
		unsigned char num,i;  
    unsigned long  Weight,AD_MAOPI,AD_MAOPI_temp;  //
		unsigned char temp[8];   //
    float a=0,b=0;
	 	s=0;
	  i=0;

	firstflag=0;
	
	Time1Iinitial();           // 
  Lcd1602Iinitial();         //
  disp_string(0,0,"12345"); //
	DispOneChar(0,1,'B');     //
  display_date(12,34,56);   //
		delay(40000);
		delay(40000);
		delay(40000);
		delay(40000);
		
//   AD_MAOPI=83886;  //
	
while(1)
{

	TR1=0; 		//启动T1 
//	LED0=~LED0;
//   HX711_Buffer = fun_total(HX711_Read_data); //
	HX711_Buffer = HX711_Read();
	LED0=~LED0;
	HX711_Buffer = HX711_Buffer/100;			   //
//	HX711_Buffer = HX711_Buffer%100000; //

//	HX711_Buffer = 84708;
	Data_sw=(HX711_Buffer/100000)%10;  //5
	Data_w=(HX711_Buffer/10000)%10;  //5
	Data_q=(HX711_Buffer/1000)%10;  //4
	Data_b=(HX711_Buffer/100)%10;  //4
	Data_s=(HX711_Buffer/10)%10;  //4;  //2
	Data_g=HX711_Buffer%10;  //1

//	DispOneChar(0,0,Data_w);   //
//	DispOneChar(1,0,Data_w);   //
//	DispOneChar(2,0,Data_w);   //
//	DispOneChar(3,0,Data_w);   //
//	DispOneChar(4,0,Data_w);   //


	DispOneChar(0,0,Data_sw+0x30);   //
	DispOneChar(1,0,Data_w+0x30); 
	DispOneChar(2,0,Data_q+0x30); 
	DispOneChar(3,0,Data_b+0x30); 
	DispOneChar(4,0,Data_s+0x30); 
	DispOneChar(5,0,Data_g+0x30); 
	
//================================	
    HC595SendData(Data_sw,1);    // 
		HC595SendData(Data_w,1);
		HC595SendData(Data_q,1);
		HC595SendData(Data_b,1); 
		HC595SendData(Data_s,1); 	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	  HC595SendData(Data_g,1);
		
		RCK_595=0; //
		_nop_();  //
		_nop_();  //
		RCK_595=1;
		RCK_595=0;
//-----------------------------------------------
 
    AD_MAOPI_temp=HX711_Buffer;   //
//  HX711_Buffer = (HX711_Buffer-84950)*100/730;  // AD TO WEIGHT
	  HX711_Buffer = (HX711_Buffer-AD_MAOPI)*100/713;  //       // 38
//	  HX711_Buffer = (HX711_Buffer-AD_MAOPI)*100/718;  //    // 46
//	  HX711_Buffer = (HX711_Buffer-AD_MAOPI)*100/745;  //    // 45
    
  Weight=HX711_Buffer;

	Data_w=(Weight/10000)%10;  //5
	Data_q=(Weight/1000)%10;  //4
	Data_b=(Weight/100)%10;  //4
	Data_s=(Weight/10)%10;  //4;  //2
	Data_g=Weight%10;  //1



	if (Data_w==0){DispOneChar(0,1,' ');}else {DispOneChar(0,1,Data_w+0x30);}  //
	DispOneChar(1,1,Data_q+0x30); 
	DispOneChar(2,1,'.'); 
	DispOneChar(3,1,Data_b+0x30); 
	DispOneChar(4,1,Data_s+0x30); 
	DispOneChar(5,1,Data_g+0x30); 
	DispOneChar(6,1,'k'); 
	DispOneChar(7,1,'g'); 

   firstflag=1;     // 

   TR1=1; 		      // 
while(firstflag)   // 

{  
	 num=KeyPro();  // 



	if(num!=0xff)  // 
   {  
		 
		 	
      TR1=0;  // 
		if('a'==num)  
		{	
      {AD_MAOPI=AD_MAOPI_temp-1;  TR1=1;}// 
		}
		
    else
		{
		 if(i==0)   disp_string(8,0,"        ");  // 
		 
     if('='==num)// 
	     {	    
						i=0;  // 
						sscanf(temp,"%f",&a); // 
						disp_string(8,0,temp);// 
						
						a=a*Weight/1000;  // 
						b = a;   //
						sprintf(temp,"%8.2f",b);    // 
						disp_string(8,1,temp);// 
						
				    firstflag=0;  // 
				    a=b=0;  // 
				 
						for(s=0;s<8;s++)  temp[s]=' ';  // 
            TR1=1;  // 
		   } 

     else if(i<8) // 
		 {
						if((1==i)&& (temp[0]=='0') )//
							{
								 if(num=='.')  //
										{
											 temp[1]='.';	//

											 DispOneChar(i+8,0,num);// 											
											 i++;
										} //这里没有判断连续按小数点，如0.0.0
				 
								 else
										 {
												temp[0]=num; // 
											 }
								 }
					 
						else								     
								{
											temp[i]=num;  // 

									DispOneChar(i+8,0,num);// 
											i++;   // 
								}
       } //else if(i<8)的括号 
		 }
  
     }//第一个if的括号 
	 
		 
		 
	 } //while(firstflag ) 的括号

		delay(40000);
//		delay(40000);

		
}}
 
//---------延时函数-------------
void  delay(unsigned int i)
{
	while(i--);
}
