/*************************************************
Copyright (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: oled.c
Author:      Version:   Date:2019.10.10
Description: ʵ�ֶ�LoRaģ���OLED��ʾ���Ķ�д������
Others:
History:

1. Date:
Author:
Modification:
*************************************************/
#include "oled.h"

extern const unsigned char g_F6x8[][6];
extern const unsigned char g_F8X16[];


/**
  * @brief OLED��Ļ��Դ���ؿ��ơ�
  * @param status
					 @arg ENABLE :��OLED��Դ��
					 @arg DSIABLE:�ر�OLED��Դ��

  * @retval None
  */
void OLED_PowerControl(uint8_t Status)
{
    if(Status == ENABLE)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    }
}

/**
  * @brief OLED��Ļд���ݡ�
  * @param type OLED����Ļ�������ͣ�
					 @arg TYPE_COMMAND :��OLED��Դ��
					 @arg TYPE_DATA:�ر�OLED��Դ��

					 data ���͸�OLED��Ļ�����ݣ�

  * @retval None
  */
void OLED_Write(uint8_t type, uint8_t data)
{
    I2CStart();
    I2CSendByte(0x78); 	//Slave address,SA0=0
    I2CWaitAck();

    if(type == TYPE_COMMAND)
    {
        I2CSendByte(0x00);	//command
    }
    else
    {
        I2CSendByte(0x40);	//data
    }
    I2CWaitAck();
    I2CSendByte(data);
    I2CWaitAck();
    I2CStop();
}

/**
  * @brief OLED��Ļ��ʾ������
  * @param None
  * @retval None
  */
void OLED_DisplayON(void)
{
    OLED_Write(TYPE_COMMAND, 0x8D);  //SET DCDC
    OLED_Write(TYPE_COMMAND, 0x14);  //DCDC ON
    OLED_Write(TYPE_COMMAND, 0xAF);  //DISPLAY ON
}

/**
  * @brief OLED��Ļ��ʾ�رա�
  * @param None
  * @retval None
  */
void OLED_DisplayOFF(void)
{
    OLED_Write(TYPE_COMMAND, 0x8D);  //SET DCDC
    OLED_Write(TYPE_COMMAND, 0x10);  //DCDC OFF
    OLED_Write(TYPE_COMMAND, 0xAE);  //DISPLAY OFF
}

/**
  * @brief OLED��Ļд���ݡ�
  * @param x 0-127��ѡ��x����λ�ã�
					 y 0-3��ѡ��y����λ�ã�
  * @retval None
  */
void OLED_SetPos(uint8_t x, uint8_t y)
{
    OLED_Write(TYPE_COMMAND, 0xb0 + y);
    OLED_Write(TYPE_COMMAND, ((x & 0xf0) >> 4) | 0x10);
    OLED_Write(TYPE_COMMAND, (x & 0x0f));
}

/**
  * @brief OLED��Ļ������
  * @param None
  * @retval None
  */
void OLED_Clear(void)
{
    uint8_t i, n;

    for(i = 0; i < 4; i++)
    {
        OLED_Write (TYPE_COMMAND, 0xb0 + i);
        OLED_Write (TYPE_COMMAND, 0x00);
        OLED_Write (TYPE_COMMAND, 0x10);
        for(n = 0; n < 128; n++)
        {
            OLED_Write(TYPE_DATA, 0);
        }
    }
}

/**
  * @brief OLED��Ļ��ʾһ���ַ���
  * @param x ����ѡ��λ�ã�
					 y ����ѡ��λ�ã�
					 chr ��Ҫ��ʾ���ַ���
					 Char_Size ��Ҫ��ʾ���ַ���С��
					 @arg 16,ѡ��8*16�����С�ַ���������ѡ��6*8�����ַ�


  * @retval None
  */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    uint8_t  c = 0, i = 0;
    c = chr - ' ';
    if(x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if(Char_Size == 16)
    {
        OLED_SetPos(x, y);
        for(i = 0; i < 8; i++)
            OLED_Write(TYPE_DATA, g_F8X16[c * 16 + i]);
        OLED_SetPos(x, y + 1);
        for(i = 0; i < 8; i++)
            OLED_Write(TYPE_DATA, g_F8X16[c * 16 + i + 8]);
    }
    else
    {
        OLED_SetPos(x, y);
        for(i = 0; i < 6; i++)
        {
            OLED_Write(TYPE_DATA, g_F6x8[c][i]);
        }
    }
}

/**
  * @brief OLED��Ļ��ʾһ���ַ�����
  * @param x ����ѡ��λ�ã�
					 y ����ѡ��λ�ã�
					 chr ��Ҫ��ʾ���ַ�����
					 Char_Size ��Ҫ��ʾ���ַ���С��

  * @retval None
  */
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size)
{
    unsigned char j = 0,x1=x;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], Char_Size);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
		for(j=0;j<x1;j=j+8)
		{
			OLED_ShowChar(j, y,' ', Char_Size);
		}
		for(j=x;j<128;j=j+8)
		{
			OLED_ShowChar(j, y,' ', Char_Size);
		}
}

/**
  * @brief OLED��Ļ��ʼ����
  * @param None
  * @retval None
  */
void OLED_Init(void)
{
    OLED_Write(TYPE_COMMAND, 0xAE);//�ر���ʾ

    OLED_Write(TYPE_COMMAND, 0xD5);//����ʱ�ӷ�Ƶ���ӣ���Ƶ��
    OLED_Write(TYPE_COMMAND, 0x80);//[3:0],��Ƶ����;[7:4],��Ƶ��

    OLED_Write(TYPE_COMMAND, 0xA8);//��������·��
    OLED_Write(TYPE_COMMAND, 0x1F);//Ĭ�� 0X1F(1/64)

    OLED_Write(TYPE_COMMAND, 0xD3);//������ʾƫ��
    OLED_Write(TYPE_COMMAND, 0x00);//Ĭ��Ϊ 0


    OLED_Write(TYPE_COMMAND, 0x40); //������ʾ��ʼ�� [5:0],����.
    OLED_Write(TYPE_COMMAND, 0xB0);

    OLED_Write(TYPE_COMMAND, 0x8D);//��ɱ�����
    OLED_Write(TYPE_COMMAND, 0x14);//bit2������

    OLED_Write(TYPE_COMMAND, 0xA1);//���ض�������,bit0:0,0->0;1,0->127;
    OLED_Write(TYPE_COMMAND, 0xC8);

    OLED_Write(TYPE_COMMAND, 0xDA);//���� COM Ӳ����������
    OLED_Write(TYPE_COMMAND, 0x00);//[5:4]����
    OLED_Write(TYPE_COMMAND, 0x81);//�Աȶ�����
    OLED_Write(TYPE_COMMAND, 0xFF);//1~255;(��������,Խ��Խ��)
    OLED_Write(TYPE_COMMAND, 0xD9);//����Ԥ�������
    OLED_Write(TYPE_COMMAND, 0x1F);//[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_Write(TYPE_COMMAND, 0xDB);//���� VCOMH ��ѹ����
    OLED_Write(TYPE_COMMAND, 0x40);//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_Write(TYPE_COMMAND, 0xA4);//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)

    OLED_Clear();//����
    OLED_Write(TYPE_COMMAND, 0xAF);//������ʾ
}
