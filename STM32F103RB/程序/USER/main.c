/* 16 * 16 的汉字就可以可以显示一排15个汉字，间隔为2
可以显示5行 ，每行间隔 20

*/


#include "stm32f10x.h"
#include "sys.h"//bit band operation
#include "SysTickDelay.h"
#include "LCM240128.h"
#include "LCD240128_CN.h"



void LCD_GPIO_INIT(void);
void pro_num(uint16_t in , uint8_t out[],uint8_t hang,uint8_t u_or_i);

uint8_t tab[]={ "ting" };
uint8_t tab1[]={ "A B C" };

uint8_t OUT[7] = {0,0,0,0,0,0,0};//注意，次数组最后一个一定为0，以便于显示时确定结束位置


uint16_t UA,UB,UC;
uint16_t IA,IB,IC;

/*----------------------------------------------------------------------------------------------*/
//显示一个汉字(16*16点阵)
//lin:行(0-7), column: 列(0-7)
//hzcode: 汉字代码(自定义的)

int main(void)
{ 	

//	uint16_t xx = 0;
  SysTick_Initaize();//us ms delay non interrupt
  LCD_GPIO_INIT();//PB PC
	
	delay_ms(100);
	SET_WR;
  SET_RD;
  SET_CD;
  REST_CE;

	
	//模拟给abc 电压电流赋值
	UA = 0;
	UB = 0;
	UC = 0;
	IA = 0;
	IB = 0;
	IC = 0;
/***************以下为title***************************/	
	
	/*******A 相电流电压Title*******************/
	LCD_Text_Init(); 
	LCD_Writestring(0,1,"A");//  	A相
	LCD_Writestring(13,1,"V");//   V电压
	LCD_Writestring(26,1,"A");//   A电流
	
	Writexthz(1,0,XIANG,3);
  Writexthz(3,0,DIAN,0);
	Writexthz(5,0,YA,  1);
	Writexthz(16,0,DIAN,0);
	Writexthz(18,0,LIU,2 );
	

	/*******B 相电流电压Title*******************/
	LCD_Writestring(0,3,"B");//  	B相
	LCD_Writestring(13,3,"V");//   V电压
	LCD_Writestring(26,3,"A");//   A电流
	
	Writexthz(1,2,XIANG,3);
  Writexthz(3,2,DIAN,0);
	Writexthz(5,2,YA,  1);
	Writexthz(16,2,DIAN,0);
	Writexthz(18,2,LIU,2 );
	
	
	/*******C 相电流电压Title*******************/
	LCD_Writestring(0,5,"C");//  	C相
	LCD_Writestring(13,5,"V");//   V电压
	LCD_Writestring(26,5,"A");//   A电流
	
	Writexthz(1,4,XIANG,3);
  Writexthz(3,4,DIAN,0);
	Writexthz(5,4,YA,  1);
	Writexthz(16,4,DIAN,0);
	Writexthz(18,4,LIU,2 );
	
	/*******电机状态？Title*******************/
	Writexthz(0,8,DIAN,0);
	Writexthz(2,8,JI,8);
	Writexthz(4,8,ZHUANG,9);
	Writexthz(6,8,TAI,10);
	LCD_Writestring(8,9,": ");//
	
	
	/*******故障类型？Title*******************/
	Writexthz(0,10,GU,4);
	Writexthz(2,10,ZHANG,5);
	Writexthz(4,10,LEI,6);
	Writexthz(6,10,XING,7);
	LCD_Writestring(8,11,": ");//
	
/***************以上为title***************************/		
 

/***************以下为变量显示，目前只显示静态***************************/		
// 	pro_num(UA,OUT,0,0);
// 	pro_num(UB,OUT,1,0);
// 	pro_num(UC,OUT,2,0);
// 	

/***************以上为变量显示，目前只显示静态***************************/		



  while(1)
	{
		
		delay_ms(500);
		UA += 1;
		UB += 2;
		UC += 3;
		IA += 1;
		IB += 2;
		IC += 3;
		pro_num(UA,OUT,0,0);
		pro_num(UB,OUT,1,0);
		pro_num(UC,OUT,2,0);
		pro_num(IA,OUT,0,1);
		pro_num(IB,OUT,1,1);
		pro_num(IC,OUT,2,1);
	
	
	}

	
}


void LCD_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	
	//直接用库函数不使用寄存器配置是因为这几个引脚分散，这样清晰
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	delay_ms(10);
}


void pro_num(uint16_t in , uint8_t out[],uint8_t hang,uint8_t u_or_i)
{
	uint8_t a = 0;
	for( a = 0;a<6;a++ )
	{
		out[0] = in/10000 + 0x30;
		out[1] = in%10000/1000+ 0x30;
		out[2] = in%1000/100+ 0x30;
		out[3] = in%100/10+ 0x30;
		out[4] = '.';
		out[5] = in%10+ 0x30;;
		out[6] = 0;
	}
	if(u_or_i == 0)
	{
		LCD_Writestring( 7,(hang*2+1),out);//特定的地方显示小数点
	}
	else
	{
		LCD_Writestring( 20,(hang*2+1),out);//特定的地方显示小数点
	}
		
	
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
