#include"ds1302.h"

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
uchar code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302时钟初始化2016年5月7日星期六12点00分00秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
uchar TIME[7] = {0, 0x36, 0x21, 0x17, 0x04, 0x06, 0x21};

unsigned char code smgduan[19]={ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 
0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F };
unsigned char DisplayData[18];

void datapros() 	 //时间读取处理转换函数
{
   	Ds1302ReadTime();
	DisplayData[0] = smgduan[TIME[2]/16];				//时
	DisplayData[1] = smgduan[TIME[2]&0x0f];				 
	DisplayData[2] = 0xbf;
	DisplayData[3] = smgduan[TIME[1]/16];				//分
	DisplayData[4] = smgduan[TIME[1]&0x0f];	
	DisplayData[5] = 0xbf;
	DisplayData[6] = smgduan[TIME[0]/16];				//秒
	DisplayData[7] = smgduan[TIME[0]&0x0f];
	
	DisplayData[8] = smgduan[TIME[6]/16];				//年
	DisplayData[9] = smgduan[TIME[6]&0x0f];				 
	DisplayData[10] = 0xbf;
	DisplayData[11] = smgduan[TIME[4]/16];				//月
	DisplayData[12] = smgduan[TIME[4]&0x0f];	
	DisplayData[13] = 0xbf;
	DisplayData[14] = smgduan[TIME[3]/16];				//日
	DisplayData[15] = smgduan[TIME[3]&0x0f];
	
	DisplayData[16] = smgduan[TIME[5]/16];				//周
	DisplayData[17] = smgduan[TIME[5]&0x0f];
}

/*---------------------------以下为设置函数，不用理会-------------------------------------------------*/

/*******************************************************************************
* 函 数 名         : Ds1302Write
* 函数功能		   : 向DS1302命令（地址+数据）
* 输    入         : addr,dat
* 输    出         : 无
*******************************************************************************/
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	RST = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	RST = 1; //然后将RST(CE)置高电平。
	_nop_();

	for (n=0; n<8; n++)//开始传送八位地址命令
	{
		DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)//写入8位数据
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	RST = 0;//传送数据结束
	_nop_();
}

/*******************************************************************************
* 函 数 名         : Ds1302Read
* 函数功能		   : 读取一个地址的数据
* 输    入         : addr
* 输    出         : dat
*******************************************************************************/

uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	RST = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	RST = 1;//然后将RST(CE)置高电平。
	_nop_();

	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)//读取8位数据
	{
		dat1 = DSIO;//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}

	RST = 0;
	_nop_();	//以下为DS1302复位的稳定时间,必须的。
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;	
}

/*******************************************************************************
* 函 数 名         : Ds1302Init
* 函数功能		   : 初始化DS1302.
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Ds1302Init()
{
	uchar n;
	Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //打开写保护功能
}

/*******************************************************************************
* 函 数 名         : Ds1302ReadTime
* 函数功能		   : 读取时钟信息
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Ds1302ReadTime()
{
	uchar n;
	for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}


