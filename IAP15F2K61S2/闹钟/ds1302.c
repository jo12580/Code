#include"ds1302.h"

//---DS1302д��Ͷ�ȡʱ����ĵ�ַ����---//
//---���ʱ�������� ���λ��дλ;-------//
uchar code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302ʱ�ӳ�ʼ��2016��5��7��������12��00��00�롣---//
//---�洢˳�������ʱ��������,�洢��ʽ����BCD��---//
uchar TIME[7] = {0, 0x36, 0x21, 0x17, 0x04, 0x06, 0x21};

unsigned char code smgduan[19]={ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 
0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F };
unsigned char DisplayData[18];

void datapros() 	 //ʱ���ȡ����ת������
{
   	Ds1302ReadTime();
	DisplayData[0] = smgduan[TIME[2]/16];				//ʱ
	DisplayData[1] = smgduan[TIME[2]&0x0f];				 
	DisplayData[2] = 0xbf;
	DisplayData[3] = smgduan[TIME[1]/16];				//��
	DisplayData[4] = smgduan[TIME[1]&0x0f];	
	DisplayData[5] = 0xbf;
	DisplayData[6] = smgduan[TIME[0]/16];				//��
	DisplayData[7] = smgduan[TIME[0]&0x0f];
	
	DisplayData[8] = smgduan[TIME[6]/16];				//��
	DisplayData[9] = smgduan[TIME[6]&0x0f];				 
	DisplayData[10] = 0xbf;
	DisplayData[11] = smgduan[TIME[4]/16];				//��
	DisplayData[12] = smgduan[TIME[4]&0x0f];	
	DisplayData[13] = 0xbf;
	DisplayData[14] = smgduan[TIME[3]/16];				//��
	DisplayData[15] = smgduan[TIME[3]&0x0f];
	
	DisplayData[16] = smgduan[TIME[5]/16];				//��
	DisplayData[17] = smgduan[TIME[5]&0x0f];
}

/*---------------------------����Ϊ���ú������������-------------------------------------------------*/

/*******************************************************************************
* �� �� ��         : Ds1302Write
* ��������		   : ��DS1302�����ַ+���ݣ�
* ��    ��         : addr,dat
* ��    ��         : ��
*******************************************************************************/
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	RST = 0;
	_nop_();

	SCLK = 0;//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1; //Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();

	for (n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)//д��8λ����
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	RST = 0;//�������ݽ���
	_nop_();
}

/*******************************************************************************
* �� �� ��         : Ds1302Read
* ��������		   : ��ȡһ����ַ������
* ��    ��         : addr
* ��    ��         : dat
*******************************************************************************/

uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	RST = 0;
	_nop_();

	SCLK = 0;//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1;//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();

	for(n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;//DS1302�½���ʱ����������
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)//��ȡ8λ����
	{
		dat1 = DSIO;//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;//DS1302�½���ʱ����������
		_nop_();
	}

	RST = 0;
	_nop_();	//����ΪDS1302��λ���ȶ�ʱ��,����ġ�
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;	
}

/*******************************************************************************
* �� �� ��         : Ds1302Init
* ��������		   : ��ʼ��DS1302.
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void Ds1302Init()
{
	uchar n;
	Ds1302Write(0x8E,0X00);		 //��ֹд���������ǹر�д��������
	for (n=0; n<7; n++)//д��7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //��д��������
}

/*******************************************************************************
* �� �� ��         : Ds1302ReadTime
* ��������		   : ��ȡʱ����Ϣ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void Ds1302ReadTime()
{
	uchar n;
	for (n=0; n<7; n++)//��ȡ7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}


