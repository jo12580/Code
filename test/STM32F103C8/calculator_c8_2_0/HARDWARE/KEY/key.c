#include "key.h"
#include "delay.h"


#define row 0
#define col 1					
#define RCC_APB2_GPIOX  RCC_APB2Periph_GPIOB//IO时钟					
#define MAX_Time   36000000 // 松手检测最大延时
#define MAX_row 5
#define MAX_col 4
#define MAX_Number (MAX_row > MAX_col)?MAX_row:MAX_col

typedef struct 
{
	GPIO_TypeDef* GPIOX;
	uint16_t GPIO_Pin;
}KGPT[2][MAX_Number]; //5*5 （）   实际是  5 * 4

KGPT KPIN={ //只需要在下面填写横竖行的IO口和管脚   IO口和管脚都可以随意定义 无需在一个IO口 管脚也无需按顺序
						{{GPIOB,GPIO_Pin_5},{GPIOB,GPIO_Pin_6} ,{GPIOB,GPIO_Pin_7}, {GPIOB,GPIO_Pin_8},{GPIOB,GPIO_Pin_9}},//row 横行
						{{GPIOA,GPIO_Pin_1},{GPIOA,GPIO_Pin_2}, {GPIOA,GPIO_Pin_3},{GPIOA,GPIO_Pin_4}}  //col 竖行
					};

					
//按键初始化函数 
void KEY_Init(void)
{
  u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2_GPIOX,ENABLE);//使能PORTC时钟
  for(i=0;i<MAX_row;i++)
	{
	 GPIO_InitStructure.GPIO_Pin = KPIN[row][i].GPIO_Pin;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成推挽输出
	 GPIO_Init(KPIN[row][i].GPIOX, &GPIO_InitStructure);//初始化row
	 GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);//每行为00000
	}
 for(i=0;i<MAX_col;i++)
	{
		 GPIO_InitStructure.GPIO_Pin  = KPIN[col][i].GPIO_Pin;//PC5
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		 GPIO_Init(KPIN[col][i].GPIOX, &GPIO_InitStructure);//初始化col
	}
} 
//行检测
u8 ReScan(GPIO_TypeDef* GPIOX,uint16_t colPin,u8 colIndex)
{
 u8 i;
 DelayMs(10);//消抖延时
 for(i = 0;i<MAX_row;i++){
  GPIO_SetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);//每个行置1
  if((GPIO_ReadInputDataBit(GPIOX, colPin) == 1)) //如果列也变了 行的值就知道了 为 i
		{
			GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin); //行恢复 置0 
		  return colIndex+i*4;//返回的数据 为0-19 对应5x4键盘的20个键
		}
	GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);     //行恢复 置0
 }
 return 0;
}
//列检测
u8 KEY_Scan(void)
{	
 u8 i,keyvalue=21;
	//u32 j=0;
	for(i = 0;i<MAX_col;i++)
	{
	 if(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0)//检测列 列值为 i
	 {
			keyvalue = ReScan(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin,i);//检测行 取键值
		  while(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0);
			//while((GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0))j++;//松手检测
			return keyvalue;//返回键值
		}
	}
return 21;
}

