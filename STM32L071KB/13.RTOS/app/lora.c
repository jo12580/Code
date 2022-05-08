#include "spi.h"
/* ������ƵƵ�ʣ�137~525MHz�� */
void LORA_SetRFFrequency(unsigned long freq)
{
  freq = (freq / 32) << 19;
  SPI_WriteRead(0x86, freq >> 16);
  SPI_WriteRead(0x87, (freq >> 8) & 0xFF);
  SPI_WriteRead(0x88, freq  & 0xFF);
}
/* ������Ƶ���ʣ�2~20dBm�� */
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
/* �����źŴ���0~9�� */
void LORA_SetBW(unsigned char bw)
{
  uint8_t ucRet;

  ucRet = SPI_WriteRead(0x1D, 0);
  ucRet &= 0x0F;
  ucRet |= bw<<4;
  SPI_WriteRead(0x9D, ucRet);
}
/* ���þ�������ʣ�1~4�� */
void LORA_SetCR(unsigned char cr)
{
  uint8_t ucRet;

  ucRet = SPI_WriteRead(0x1D, 0);
  ucRet &= 0xF1;
  ucRet |= cr<<1;
  SPI_WriteRead(0x9D, ucRet);
}
/* LoRa��ʼ�� */
void LORA_Init(void)
{
  SPI_WriteRead(0x81, 0);							/* ����˯��ģʽ */
  SPI_WriteRead(0x81, 0x80);					/* ����LoRaģʽ */
  SPI_WriteRead(0x81, 1);							/* ���ô���ģʽ */
  LORA_SetRFFrequency(434);						/* ������ƵƵ�ʣ�137~525MHz�� */
  LORA_SetRFPower(10);								/* ������Ƶ���ʣ�2~20dBm�� */
  SPI_WriteRead(0x9E, 7<<4);					/* ������Ƶ���ӣ�7~12�� */
  LORA_SetBW(7);											/* �����źŴ���0~9�� */
  LORA_SetCR(1);											/* ���þ�������ʣ�1~4�� */
  SPI_WriteRead(0x81, 5);							/* ������������ģʽ */
}
/* LoRa����: pucBuf-�������ݣ�ucSize-���ݸ��� */
void LORA_Tx(unsigned char *pucBuf, unsigned char ucSize)
{
  unsigned int i;
  unsigned char ret;

  SPI_WriteRead(0x81, 1);							/* ���ô���ģʽ */
  ret = SPI_WriteRead(0x0E, 0);				/* ��ȡFifoTxBaseAddr */
  SPI_WriteRead(0x8D, ret);						/* ����FifoAddrPtr */
  SPI_WriteRead(0xA2, ucSize);				/* ����PayloadLength */
  for(i=0; i<ucSize; i++)							/* д���ݵ�FIFO */
    SPI_WriteRead(0x80, pucBuf[i]);
  SPI_WriteRead(0x81, 3);							/* ���÷���ģʽ */
  i = 65535;
  do {
    ret = SPI_WriteRead(0x12, 0);			/* ����־ */
		i--;
  }while(((ret & 8) == 0) && (i != 0));	/* �ȴ�������� */
  SPI_WriteRead(0x92, 8);							/* ���������� */
  SPI_WriteRead(0x81, 5);							/* ������������ģʽ */
}
/* Lora����: pucBuf-�������ݣ�����ֵ-���ݸ��� */
unsigned char LORA_Rx(unsigned char *pucBuf)
{
  unsigned char i, ret;

  ret = SPI_WriteRead(0x12, 0);				/* ����־ */
  if(ret & 0x40)											/* ������� */
  {
    SPI_WriteRead(0x81, 1);						/* ���ô���ģʽ */
    SPI_WriteRead(0x92, 0x40);				/* ���������� */
    ret = SPI_WriteRead(0x10, 0);			/* ��ȡFifoRxCurrentAddr */
    SPI_WriteRead(0x8D, ret);					/* ����FifoAddrPtr */
    ret = SPI_WriteRead(0x13, 0);			/* ��ȡRxNbBytes */
    for(i=0; i<ret; i++)
      pucBuf[i] = SPI_WriteRead(0, 0);/* ��FIFO������ */
    SPI_WriteRead(0x81, 5);						/* ������������ģʽ */
  }
  else
    ret = 0;
  return ret;
}
