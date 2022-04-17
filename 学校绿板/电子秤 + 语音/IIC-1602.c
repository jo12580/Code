//#include <reg52.h>
#include <IIC-1602.h>


//=============delay time=====================
void delay1ms(int i)  // ����Ƶ��=11.0592Mhz ��λ��ʱ1ms
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
//lcd��ʼ��
void Lcd1602Iinitial()
{
	start_flag();   //��ʼ��־
	transfer(0x7C); //ѡ��SLAVE ADDRESS
	transfer(0x00); //�����ֽڣ�Co=0,A0=0,��ʾ���´����N ���ֽ���ָ��
	transfer(0x28); // Function set 
	delay1ms(5);
	transfer(0x0c); // Display ON/OFF
	delay1ms(5);
	transfer(0x01); // Clear display
	delay1ms(10);
	transfer(0x06); // set display mode
	stop_flag();    //������־
}


/*��ָ���к���λ����ʾָ������ĸ�����֣�5*7 ����ģ�*/


//void disp_string(int column,int line,unsigned char *dp)
//{
//	int i;
//	start_flag();
//	transfer(0x7C); //ѡ��SLAVE ADDRESS
//	transfer(0x80); /*�����ֽڣ�Co=1,A0=0,��ʾ���´����1 ���ֽ���ָ��*/
//	transfer(0x80+(line)*0x40+(column)); //����DDRAM ADDRESS���ڼ��У��ڼ���
//	transfer(0x40); /*�����ֽڣ�Co=0,A0=1,��ʾ���´����n ���ֽ�������*/
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
 ��ʾ��궨λ
=======================================================*/
void Disp_XY( char posx,char posy)
{
    unsigned char temp;
    temp=posx;                          //&0x1f; ԭΪ���Ƶ�����
    posy&=0x1;
    if ( posy )temp|=0xc0;
         else temp|=0x80;
    	transfer(temp);
}



/*=======================================================
 ��ָ��λ����ʾһ���ַ�
=======================================================*/
void DispOneChar(unsigned char x,unsigned char y,unsigned char Wdata)
{   
	start_flag();
	transfer(0x7C); //ѡ��SLAVE ADDRESS
	transfer(0x80); /*�����ֽڣ�Co=1,A0=0,��ʾ���´����1 ���ֽ���ָ��*/
    Disp_XY( x, y );                            // ��λ��ʾ��ַ
    transfer(0x40); /*�����ֽڣ�Co=0,A0=1,��ʾ���´����n ���ֽ�������*/
    transfer( Wdata );                      // д�ַ�
	stop_flag();
}
//=========================================================================
