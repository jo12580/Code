//#include <reg52.h>
#include <IIC-1602.h>


//=============delay time=====================
void delay1ms(int i)  // 晶振频率=11.0592Mhz 单位延时1ms
{
	int j,k;
	for(j=0;j<i;j++)
	for(k=0;k<110;k++);
}


void transfer(int data1)
{
	int i;
	for(i=0;i<8;i++)
		{
		scl_1602=0;
		if(data1&0x80) sda_1602=1; else sda_1602=0;
			scl_1602=1;
			scl_1602=0;
		data1=data1<<1;
		}
			sda_1602=0;
			scl_1602=1;
			scl_1602=0;
}
void start_flag()
{
	scl_1602=1; //START FLAG
	sda_1602=1; //START FLAG
	sda_1602=0; //START FLAG
}
void stop_flag()
{
	scl_1602=1; //STOP FLAG
	sda_1602=0; //STOP FLAG
	sda_1602=1; //STOP FLAG
}
//lcd初始化
void Lcd1602Iinitial()
{
	start_flag();   //开始标志
	transfer(0x7C); //选择SLAVE ADDRESS
	transfer(0x00); //控制字节：Co=0,A0=0,表示以下传输的N 个字节是指令
	transfer(0x28); // Function set 
	delay1ms(5);
	transfer(0x0c); // Display ON/OFF
	delay1ms(5);
	transfer(0x01); // Clear display
	delay1ms(10);
	transfer(0x06); // set display mode
	stop_flag();    //结束标志
}


/*在指定行和列位置显示指定的字母、数字（5*7 点阵的）*/


//void disp_string(int column,int line,unsigned char *dp)
//{
//	int i;
//	start_flag();
//	transfer(0x7C); //选择SLAVE ADDRESS
//	transfer(0x80); /*控制字节：Co=1,A0=0,表示以下传输的1 个字节是指令*/
//	transfer(0x80+(line)*0x40+(column)); //设置DDRAM ADDRESS：第几行，第几列
//	transfer(0x40); /*控制字节：Co=0,A0=1,表示以下传输的n 个字节是数据*/
//	for(i=0;i<16;i++)
//	{
//		transfer(*dp);
//		dp=dp+1;
//	}
//	stop_flag();
//}

void disp_string(uchar x,uchar y,uchar *s)
  {    
       
  while (*s)
   {    
   DispOneChar(x,y,*s);    
   s++;
   x++;    
   }
  }


/*=======================================================
 显示光标定位
=======================================================*/
void Disp_XY( char posx,char posy)
{
    unsigned char temp;
    temp=posx;                          //&0x1f; 原为限制的条件
    posy&=0x1;
    if ( posy )temp|=0xc0;
         else temp|=0x80;
    	transfer(temp);
}



/*=======================================================
 按指定位置显示一个字符
=======================================================*/
void DispOneChar(unsigned char x,unsigned char y,unsigned char Wdata)
{   
	start_flag();
	transfer(0x7C); //选择SLAVE ADDRESS
	transfer(0x80); /*控制字节：Co=1,A0=0,表示以下传输的1 个字节是指令*/
    Disp_XY( x, y );                            // 定位显示地址
    transfer(0x40); /*控制字节：Co=0,A0=1,表示以下传输的n 个字节是数据*/
    transfer( Wdata );                      // 写字符
	stop_flag();
}
//=========================================================================
