#include "spi.h"
/* 设置射频频率（137~525MHz） */
void LORA_SetRFFrequency(unsigned long freq)
{
  freq = (freq / 32) << 19;
  SPI_WriteRead(0x86, freq >> 16);
  SPI_WriteRead(0x87, (freq >> 8) & 0xFF);
  SPI_WriteRead(0x88, freq  & 0xFF);
}
/* 设置射频功率（2~20dBm） */
void LORA_SetRFPower(unsigned char power)
{
  power -= 2;
  if(power > 15)
  {
    power -= 3;
    SPI_WriteRead(0xCD, 0x87);
  }
  SPI_WriteRead(0x89, power|0x80);
}
/* 设置信号带宽（0~9） */
void LORA_SetBW(unsigned char bw)
{
  uint8_t ucRet;

  ucRet = SPI_WriteRead(0x1D, 0);
  ucRet &= 0x0F;
  ucRet |= bw<<4;
  SPI_WriteRead(0x9D, ucRet);
}
/* 设置纠错编码率（1~4） */
void LORA_SetCR(unsigned char cr)
{
  uint8_t ucRet;

  ucRet = SPI_WriteRead(0x1D, 0);
  ucRet &= 0xF1;
  ucRet |= cr<<1;
  SPI_WriteRead(0x9D, ucRet);
}
/* LoRa初始化 */
void LORA_Init(void)
{
  SPI_WriteRead(0x81, 0);							/* 设置睡眠模式 */
  SPI_WriteRead(0x81, 0x80);					/* 设置LoRa模式 */
  SPI_WriteRead(0x81, 1);							/* 设置待机模式 */
  LORA_SetRFFrequency(434);						/* 设置射频频率（137~525MHz） */
  LORA_SetRFPower(10);								/* 设置射频功率（2~20dBm） */
  SPI_WriteRead(0x9E, 7<<4);					/* 设置扩频因子（7~12） */
  LORA_SetBW(7);											/* 设置信号带宽（0~9） */
  LORA_SetCR(1);											/* 设置纠错编码率（1~4） */
  SPI_WriteRead(0x81, 5);							/* 设置连续接收模式 */
}
/* LoRa发送: pucBuf-发送数据，ucSize-数据个数 */
void LORA_Tx(unsigned char *pucBuf, unsigned char ucSize)
{
  unsigned int i;
  unsigned char ret;

  SPI_WriteRead(0x81, 1);							/* 设置待机模式 */
  ret = SPI_WriteRead(0x0E, 0);				/* 读取FifoTxBaseAddr */
  SPI_WriteRead(0x8D, ret);						/* 设置FifoAddrPtr */
  SPI_WriteRead(0xA2, ucSize);				/* 设置PayloadLength */
  for(i=0; i<ucSize; i++)							/* 写数据到FIFO */
    SPI_WriteRead(0x80, pucBuf[i]);
  SPI_WriteRead(0x81, 3);							/* 设置发送模式 */
  i = 65535;
  do {
    ret = SPI_WriteRead(0x12, 0);			/* 读标志 */
		i--;
  }while(((ret & 8) == 0) && (i != 0));	/* 等待发送完成 */
  SPI_WriteRead(0x92, 8);							/* 清除发送完成 */
  SPI_WriteRead(0x81, 5);							/* 设置连续接收模式 */
}
/* Lora接收: pucBuf-接收数据，返回值-数据个数 */
unsigned char LORA_Rx(unsigned char *pucBuf)
{
  unsigned char i, ret;

  ret = SPI_WriteRead(0x12, 0);				/* 读标志 */
  if(ret & 0x40)											/* 接收完成 */
  {
    SPI_WriteRead(0x81, 1);						/* 设置待机模式 */
    SPI_WriteRead(0x92, 0x40);				/* 清除接收完成 */
    ret = SPI_WriteRead(0x10, 0);			/* 读取FifoRxCurrentAddr */
    SPI_WriteRead(0x8D, ret);					/* 设置FifoAddrPtr */
    ret = SPI_WriteRead(0x13, 0);			/* 读取RxNbBytes */
    for(i=0; i<ret; i++)
      pucBuf[i] = SPI_WriteRead(0, 0);/* 从FIFO读数据 */
    SPI_WriteRead(0x81, 5);						/* 设置连续接收模式 */
  }
  else
    ret = 0;
  return ret;
}
