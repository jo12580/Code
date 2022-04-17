// #include "stm32f10x.h"
#include "sys.h"//bit band operation
#include "SysTickDelay.h"
#include "LCM240128.h"
#include "lcd_graph.h"
#include "main.h"


#define Delay_count 5

// uint8_t ttab[][32] = 
// {
// 	
// 0x00,0x40,0x37,0x10,0x83,0x60,0x2f,0x08,
// 0x13,0x22,0xe3,0x22,0x23,0x22,0x22,0x22,
// 0x40,0x48,0xfc,0x40,0xf8,0x40,0xfe,0x08,
// 0xfc,0x08,0xf8,0x08,0xf8,0x08,0x28,0x10,

// };


//函数功能:
//检测状态字，判断STA.0 STA.1是否准备好，为读写单个字做准备
uint8_t LCD_TestStaBit01(void)
{   
  uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD;
		
    REST_RD;
		
		//自己添加的延时，要求大于150ns********************************************
		//nop12();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x03 );
    SET_RD;
  } 
  while(sta!=0x03); 
  return sta; 
}

//函数功能:
//检测状态字，判断STA.2 是否准备好，为读连续字做准备
uint8_t LCD_TestStaBit02(void)
{   
  uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD; 
    REST_RD;
		
		//自己添加的延时，要求大于150ns********************************************tacc
		//nop12();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x04 );
    SET_RD;
  } 
  while(sta!=0x04); 
  return sta;   
}

//函数功能:
//检测状态字，判断STA.3 是否准备好，为写连续字做准备
uint8_t LCD_TestStaBit03(void)
{   
 uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD; 
    REST_RD;
		
		//自己添加的延时，要求大于150ns********************************************tacc
		//nop12();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x08 ) ;
    SET_RD;
  } 
  while(sta!=0x08); 
  return sta;  
}

//函数功能:
//检测状态字，为拷屏做准备
uint8_t LCD_TestStaBit06(void)
{   
 uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD; 
    REST_RD;
		
		//自己添加的延时，要求大于80ns********************************************tacc
		//nop7();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x08 );
    SET_RD;

  } 
  while(sta!=0x08); 
  return sta;  
}




//函数功能:
//写入数据
void LCD_Writedata(uint8_t dat) 
{ 
  LCD_TestStaBit01(); 
  REST_CD; 
  REST_WR; 
	
 //设置数据口为输出方向
 //读取数据( 假设这里用PB0 - PB7 )
  Dirout;
  Dataout(dat); 
	
	//自己添加的延时，要求大于40ns********************************************tdh
	//nop3();
	delay_us(Delay_count);
	
  SET_WR; 
  SET_CD; 
	
	//自己添加的延时，要求大于40ns********************************************tacc
	//	nop3();
	delay_us(Delay_count);
}

//函数功能：
//向240128中写入指令
void LCD_Writecom(uint8_t command) 
{ 
  LCD_TestStaBit01(); 
  SET_CD; 
  REST_WR; 
  Dirout;
  Dataout(command);
  SET_WR; 
  SET_CD; 
	
	//自己添加的延时，要求大于40ns********************************************tacc
	//	nop3();
	delay_us(Delay_count);
}

//函数功能：
//向240128中写入单个字节
void LCD_Writesingle(uint8_t dat,uint8_t command) 
{ 
  LCD_Writedata(dat); 
  LCD_Writecom(command); 
}

//函数功能：
//向240128中写入双个字节
void LCD_Writedouble(uint8_t data1,uint8_t data2,uint8_t command) 
{ 
  LCD_Writedata(data1); 
  LCD_Writedata(data2); 
  LCD_Writecom(command); 
}

//函数功能
//清空LCD中存储的内容
void LCD_Clear(void) 
{ 
  uint16_t i,j; 
  LCD_Writedouble(0x00,0x08,LCD_ADR_POS); //注意测试
  LCD_Writecom(LCD_AUT_WR); 
  for(j=0;j<128;j++) 
    for(i=0;i<30;i++) 
    { 
      LCD_Writedata(0x00); 
    } 
  LCD_Writecom(LCD_AUT_OVR); 
}


//函数功能
//文本方式初始化
void LCD_Text_Init(void) 
{ 
  uint i,j; 
  LCD_Writedouble(0x00,0x02,0x21); //设置光标位置
  LCD_Writedouble(0x00,0x00,0x40);  //文本区起始地址
  LCD_Writedouble(0x1e,0x00,0x41);  //文本区宽度
 // LCD_Writedouble(0x00,0x08,0x42); //图形区起始地址
 // LCD_Writedouble(0x1e,0x00,0x43);  //图形区宽度
  LCD_Writecom(0x94); //
  LCD_Writecom(0xa7);   
  LCD_Writecom(0x80); 
  LCD_Writedouble(0x00,0x00,0x24); 
  
	LCD_Writecom(0xb0);  
  for(j=0;j<240;j++) 
    for(i=0;i<30;i++) 
    { 
      LCD_Writedata(0x00); 
    } 
  delay_us(300); ///////////////////////////////////////这个延时要增长？
  LCD_Writecom(0xb2); 
}

//函数功能
//写入字符串
void LCD_Writestring(uchar x,uchar y,uchar *s) 
{ 
  uint j; 
  j=30*y+x; 
  LCD_Writedouble(j%256,j/256,LCD_ADR_POS); 
  LCD_Writecom(LCD_AUT_WR); 
  while(*s!=0) 
  {
    LCD_Writedata(*s-0x20);
    s++;
  } 
  delay_us(100); 
  LCD_Writecom(LCD_AUT_OVR); 
}






// void wrcgram(uint8_t *p)           //字模进入CGRAM子程序
// {
//    uint8_t i;
//  	 uint16_t lcdadress;       //lcd地址
//    uint8_t lcdh,lcdl;        //lcd地址高低
// 	
// 	 for(i=0;i<17;i++)
//    {
//      lcdadress=30*i+1;
//      	lcdh=lcdadress/256;
// 	   lcdl=lcdadress-lcdh*256;
//     	LCD_Writedouble(lcdl,lcdh,0x24);
//      LCD_Writesingle(0x01,0xc0);         //在文本区显示CGROM字符
//     }
// 		
// 		
// 	 LCD_Writedouble(0x00,0x24,0x24);   //CGRAM首地址
// 	 LCD_Writecom(0xb0);
//      for(i=0;i<16;i++)           //左半部 代码，代码号80H,81H
// 	 {
// 	  LCD_Writedata(p[i*2]);
//      }
// 	 for(i=0;i<16;i++)
// 	 { 
// 	  LCD_Writedata(p[i*2+1]);     //右半部 代码，代码号 82H，83H
// 	 }
// 	 delay_us(300);
// 	 LCD_Writecom(0xb2);

// }
// void dispchn2(uint8_t x,uint8_t y)        //显示汉字，通过输出CGRAM代码
// {
// 	uint16_t lcdadress;       //lcd地址
//   uint8_t lcdh,lcdl;        //lcd地址高低
// 	
// 	lcdadress=x*30+y;
// 	lcdh=lcdadress/256;
// 	lcdl=lcdadress-lcdh*256;
// 	LCD_Writedouble(lcdl,lcdh,0x24);


// 	LCD_Writesingle(0x80,0xc0);
// 	LCD_Writesingle(0x82,0xc0);
// 	lcdadress=(x+1)*30+y;
// 	lcdh=lcdadress/256;
// 	lcdl=lcdadress-lcdh*256;
// 	LCD_Writedouble(lcdl,lcdh,0x24);
// 	
// 	LCD_Writesingle(0x81,0xc0);
// 	LCD_Writesingle(0x83,0xc0);

// }


//自定义字符CGRAM
void cgram(unsigned char a[][32],uint8_t num)//num为此汉字的编号 0 - 31
{
	uint8_t j;
	uint16_t address;
	uint8_t address_a,address_b;			//a为高8位，b为低8位  
	address = 0x1c00 + (0x20*num);
	address_a = (uint8_t)(address>>8);
	address_b = (uint8_t)(address);
	
	LCD_Writedouble(0x03,0x00,0x22);//设置CGRAM的偏置地址，此时，GRAM从0x1c00 开始，coder从0x80开始
	
	LCD_Writedouble(address_b,address_a,0x24);
	LCD_Writecom(0xb0);
	//for(i = 0;i<10;i++)
//	{
		for(j=0;j<32;j++)
		{
			//LCD_TestStaBit03();
			LCD_Writedata(a[0][j]);
		}
		delay_us(100);///////////////////////////////////////////////////////////这里没有判断忙，直接大延时，不知为什么判断忙过不去
		LCD_Writecom(0xb2);
//	}
}

//文本写入汉字
void Writexthz(uint8_t x,uint8_t y,uint8_t a[][32],uint8_t num)
{
	uint8_t coder;
	uint16_t j;
	cgram(a,num);
	j = y * 30 + x;
	coder = 0x80 + (4*num);
	LCD_Writedouble(j%256,j/256,0x24);
	LCD_Writecom(0xb0);
	LCD_Writedata(coder);
	LCD_Writedata(coder+2);
	delay_us(300);
	LCD_Writecom(0xb2);
	
	j += 30;
	LCD_Writedouble(j%256,j/256,0x24);
	LCD_Writecom(0xb0);
	LCD_Writedata(coder+1);
	LCD_Writedata(coder+3);
	
	delay_us(100);
	
	LCD_Writecom(0xb2);
}





//函数功能
//写入图片
//比如想写入240*128大小的图片 a=240 b=128
//x y 表示想要写入的坐标
void LCD_Writegraph(uchar a,uchar b,uchar x,uchar y,const uchar *ttab)
{
  uchar i,j;
  uint add;
  uchar add_x,add_y;
  uint k;
  add_x=x;
  for(i=0;i<b;i++)
  {
    add_y=y+i;			 
    add=30*add_y+add_x+0x0800;	
    LCD_Writedouble(add%256,add/256,LCD_ADR_POS); 
   // LCD_Writecom(LCD_AUT_WR);
    for(j=0;j<(a/8);j++)
    {
      k=j*b;
      //LCD_Writedata(*(ttab+k+i));
      LCD_Writesingle(*(ttab+k+i),0xc0);
    }
   // LCD_Writecom(LCD_AUT_OVR);
  }
}

//函数功能
//写入单个图表
//b为写入图表的y轴坐标【0.。120】  x为写入图表的x轴坐标【0.。29】
void LCD_Writebar_ch(uchar b,uchar x,const uchar *ttab)
{
  uchar i,j,y,a;
  uint add;
  uchar add_x,add_y;
  uint k;
  add_x=x;
  a=8;
  y=128-b-8;
  for(i=0;i<b;i++)
  {
    add_y=y+i;			 
    add=30*add_y+add_x+0x0800;	
    LCD_Writedouble(add%256,add/256,LCD_ADR_POS); 
    for(j=0;j<(a/8);j++)
    {
      k=j*b;
      LCD_Writesingle(*(ttab+k+i),0xc0);
    }
  }
}

//函数功能
//写入单个图表
//b为写入图表的y轴坐标【0.。120】  x为写入图表的x轴坐标【0.。29】
void LCD_Writebar(uint *dat)
{
  uchar i; 
  LCD_Clear(); 
  LCD_Writecom(LCD_DIS_MD6);  //切换到图形
  LCD_Writegraph(240,8,1,120,graph_cur_x);
  LCD_Writegraph(8,128,0,0,graph_cur_y);
  for(i=0;i<29;i++)
  {
    LCD_Writebar_ch((uchar)(((*(dat+i))*2)/100),i+1,graph_tab);
  } 
}

void LCD_Welcome(void)
{
  LCD_Writecom(LCD_DIS_MD6);
  LCD_Writegraph(240,128,0,0,graph_wel_1);
  HAL_Delay(2000);
 // LCD_Clear();
 // LCD_Writegraph(240,128,0,0,graph_wel_2);
 // HAL_Delay(2000);
  //LCD_Clear();
}

