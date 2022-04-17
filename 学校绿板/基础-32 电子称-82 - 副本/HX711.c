#include "HX711.h"


//****************************************************
//延时函数
//****************************************************
void Delay__hx711_us(void)
{
	_nop_();
	_nop_();

}

//****************************************************
//读取HX711
//****************************************************
unsigned long HX711_Read(void)	//增益128
{
	
	
	unsigned long count; 
	unsigned char i; 
	unsigned int j=1;
  	HX711_DOUT=1; 
	  Delay__hx711_us();
  	HX711_SCK=0; 
  	count=0; 
//	  while(HX711_DOUT }; 
   	while(HX711_DOUT && j ) {j++;}; ///
  	for(i=0;i<24;i++)
           
	{ 
	  	HX711_SCK=1; 
	  	count=count<<1; 
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
	} 
 	HX711_SCK=1; 
  count=count^0x800000;///
	Delay__hx711_us();
	HX711_SCK=0;  
	return(count);
		
}


//unsigned long  fun_total(unsigned long  * array)		//滤除不正常值
//{
// 	unsigned long  min,max;
// 	unsigned long  total = 0;//返回的平均值
// 	unsigned char i;  
// 	min=HX711_Read();
////	min=min>>4;
// 	max=min;
// 	for(i=0; i<10; i++)
// 	{
//		array[i]=HX711_Read();
////		array[i]=array[i]>>4;
// 		if( array[i] < min )  min = array[i]; 
// 		if( array[i] > max )  max = array[i]; 
// 		total += array[i];
// 	}
// 	total = total - min - max;
// 	total = total / 8.0 ;
// 	return total;
//}



