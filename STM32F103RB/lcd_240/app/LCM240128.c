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


//��������:
//���״̬�֣��ж�STA.0 STA.1�Ƿ�׼���ã�Ϊ��д��������׼��
uint8_t LCD_TestStaBit01(void)
{   
  uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD;
		
    REST_RD;
		
		//�Լ���ӵ���ʱ��Ҫ�����150ns********************************************
		//nop12();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x03 );
    SET_RD;
  } 
  while(sta!=0x03); 
  return sta; 
}

//��������:
//���״̬�֣��ж�STA.2 �Ƿ�׼���ã�Ϊ����������׼��
uint8_t LCD_TestStaBit02(void)
{   
  uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD; 
    REST_RD;
		
		//�Լ���ӵ���ʱ��Ҫ�����150ns********************************************tacc
		//nop12();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x04 );
    SET_RD;
  } 
  while(sta!=0x04); 
  return sta;   
}

//��������:
//���״̬�֣��ж�STA.3 �Ƿ�׼���ã�Ϊд��������׼��
uint8_t LCD_TestStaBit03(void)
{   
 uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD; 
    REST_RD;
		
		//�Լ���ӵ���ʱ��Ҫ�����150ns********************************************tacc
		//nop12();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x08 ) ;
    SET_RD;
  } 
  while(sta!=0x08); 
  return sta;  
}

//��������:
//���״̬�֣�Ϊ������׼��
uint8_t LCD_TestStaBit06(void)
{   
 uint8_t sta;
  do
  {
		Dirout;
		Dataout(0xff);
    SET_CD; 
    REST_RD;
		
		//�Լ���ӵ���ʱ��Ҫ�����80ns********************************************tacc
		//nop7();
		delay_us(Delay_count);
		
		Dirin;
		sta = ( (Datain())&0x08 );
    SET_RD;

  } 
  while(sta!=0x08); 
  return sta;  
}




//��������:
//д������
void LCD_Writedata(uint8_t dat) 
{ 
  LCD_TestStaBit01(); 
  REST_CD; 
  REST_WR; 
	
 //�������ݿ�Ϊ�������
 //��ȡ����( ����������PB0 - PB7 )
  Dirout;
  Dataout(dat); 
	
	//�Լ���ӵ���ʱ��Ҫ�����40ns********************************************tdh
	//nop3();
	delay_us(Delay_count);
	
  SET_WR; 
  SET_CD; 
	
	//�Լ���ӵ���ʱ��Ҫ�����40ns********************************************tacc
	//	nop3();
	delay_us(Delay_count);
}

//�������ܣ�
//��240128��д��ָ��
void LCD_Writecom(uint8_t command) 
{ 
  LCD_TestStaBit01(); 
  SET_CD; 
  REST_WR; 
  Dirout;
  Dataout(command);
  SET_WR; 
  SET_CD; 
	
	//�Լ���ӵ���ʱ��Ҫ�����40ns********************************************tacc
	//	nop3();
	delay_us(Delay_count);
}

//�������ܣ�
//��240128��д�뵥���ֽ�
void LCD_Writesingle(uint8_t dat,uint8_t command) 
{ 
  LCD_Writedata(dat); 
  LCD_Writecom(command); 
}

//�������ܣ�
//��240128��д��˫���ֽ�
void LCD_Writedouble(uint8_t data1,uint8_t data2,uint8_t command) 
{ 
  LCD_Writedata(data1); 
  LCD_Writedata(data2); 
  LCD_Writecom(command); 
}

//��������
//���LCD�д洢������
void LCD_Clear(void) 
{ 
  uint16_t i,j; 
  LCD_Writedouble(0x00,0x08,LCD_ADR_POS); //ע�����
  LCD_Writecom(LCD_AUT_WR); 
  for(j=0;j<128;j++) 
    for(i=0;i<30;i++) 
    { 
      LCD_Writedata(0x00); 
    } 
  LCD_Writecom(LCD_AUT_OVR); 
}


//��������
//�ı���ʽ��ʼ��
void LCD_Text_Init(void) 
{ 
  uint i,j; 
  LCD_Writedouble(0x00,0x02,0x21); //���ù��λ��
  LCD_Writedouble(0x00,0x00,0x40);  //�ı�����ʼ��ַ
  LCD_Writedouble(0x1e,0x00,0x41);  //�ı������
 // LCD_Writedouble(0x00,0x08,0x42); //ͼ������ʼ��ַ
 // LCD_Writedouble(0x1e,0x00,0x43);  //ͼ�������
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
  delay_us(300); ///////////////////////////////////////�����ʱҪ������
  LCD_Writecom(0xb2); 
}

//��������
//д���ַ���
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






// void wrcgram(uint8_t *p)           //��ģ����CGRAM�ӳ���
// {
//    uint8_t i;
//  	 uint16_t lcdadress;       //lcd��ַ
//    uint8_t lcdh,lcdl;        //lcd��ַ�ߵ�
// 	
// 	 for(i=0;i<17;i++)
//    {
//      lcdadress=30*i+1;
//      	lcdh=lcdadress/256;
// 	   lcdl=lcdadress-lcdh*256;
//     	LCD_Writedouble(lcdl,lcdh,0x24);
//      LCD_Writesingle(0x01,0xc0);         //���ı�����ʾCGROM�ַ�
//     }
// 		
// 		
// 	 LCD_Writedouble(0x00,0x24,0x24);   //CGRAM�׵�ַ
// 	 LCD_Writecom(0xb0);
//      for(i=0;i<16;i++)           //��벿 ���룬�����80H,81H
// 	 {
// 	  LCD_Writedata(p[i*2]);
//      }
// 	 for(i=0;i<16;i++)
// 	 { 
// 	  LCD_Writedata(p[i*2+1]);     //�Ұ벿 ���룬����� 82H��83H
// 	 }
// 	 delay_us(300);
// 	 LCD_Writecom(0xb2);

// }
// void dispchn2(uint8_t x,uint8_t y)        //��ʾ���֣�ͨ�����CGRAM����
// {
// 	uint16_t lcdadress;       //lcd��ַ
//   uint8_t lcdh,lcdl;        //lcd��ַ�ߵ�
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


//�Զ����ַ�CGRAM
void cgram(unsigned char a[][32],uint8_t num)//numΪ�˺��ֵı�� 0 - 31
{
	uint8_t j;
	uint16_t address;
	uint8_t address_a,address_b;			//aΪ��8λ��bΪ��8λ  
	address = 0x1c00 + (0x20*num);
	address_a = (uint8_t)(address>>8);
	address_b = (uint8_t)(address);
	
	LCD_Writedouble(0x03,0x00,0x22);//����CGRAM��ƫ�õ�ַ����ʱ��GRAM��0x1c00 ��ʼ��coder��0x80��ʼ
	
	LCD_Writedouble(address_b,address_a,0x24);
	LCD_Writecom(0xb0);
	//for(i = 0;i<10;i++)
//	{
		for(j=0;j<32;j++)
		{
			//LCD_TestStaBit03();
			LCD_Writedata(a[0][j]);
		}
		delay_us(100);///////////////////////////////////////////////////////////����û���ж�æ��ֱ�Ӵ���ʱ����֪Ϊʲô�ж�æ����ȥ
		LCD_Writecom(0xb2);
//	}
}

//�ı�д�뺺��
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





//��������
//д��ͼƬ
//������д��240*128��С��ͼƬ a=240 b=128
//x y ��ʾ��Ҫд�������
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

//��������
//д�뵥��ͼ��
//bΪд��ͼ���y�����꡾0.��120��  xΪд��ͼ���x�����꡾0.��29��
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

//��������
//д�뵥��ͼ��
//bΪд��ͼ���y�����꡾0.��120��  xΪд��ͼ���x�����꡾0.��29��
void LCD_Writebar(uint *dat)
{
  uchar i; 
  LCD_Clear(); 
  LCD_Writecom(LCD_DIS_MD6);  //�л���ͼ��
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

