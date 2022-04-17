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
#include "i2c.h"
#include "oled.h"
#include "font.h"

/* ����λ�ã��ο���3.6�� */
void OLED_SetPos(unsigned char x, unsigned char y)
{
	OLED_Write(TYPE_COMMAND, x & 0x0f);
	OLED_Write(TYPE_COMMAND, 0x10 + ((x & 0xf0) >> 4));
	OLED_Write(TYPE_COMMAND, 0xB0 + y);
}
/* �����Ļ */
void OLED_Clear(void)
{
	unsigned char i, j;

	for(i = 0; i < 4; i++)
	{
		OLED_SetPos(0, i);
		for(j = 0; j < 128; j++)
			OLED_Write(TYPE_DATA, 0);
	}
}
/**
  * @brief OLED��Ļ��ʼ����
  * @param None
  * @retval None
  */
void OLED_Init(void)
{
	OLED_Write(TYPE_COMMAND, 0xA1);	/* ����ӳ��, b0:0,0->0(��λֵ);1,0->127 */

	OLED_Write(TYPE_COMMAND, 0xA8);	/* ���ñ� */
	OLED_Write(TYPE_COMMAND, 0x1F);	/* 0F~3F,16MUX~64MUX(��λֵ) */

	OLED_Write(TYPE_COMMAND, 0xC8);	/* COMɨ�跽��, b3:0,����ģʽ(��λֵ);b3:1,��תģʽ */

	OLED_Write(TYPE_COMMAND, 0xDA);	/* COM�������� */
	OLED_Write(TYPE_COMMAND, 0x00);	/* D4:0,˳��;1,����(��λֵ) */

	OLED_Write(TYPE_COMMAND, 0x8D);	/* ���� */
	OLED_Write(TYPE_COMMAND, 0x14);	/* D2:0,��ֹ(��λֵ);1,���� */

	OLED_Clear();										/* �����Ļ */
	OLED_Write(TYPE_COMMAND, 0xAF);	/* ������ʾ */
}

/**
  * @brief OLED��Ļ��ʾһ���ַ���
  * @param x ����ѡ��λ�ã�
					 y ����ѡ��λ�ã�
					 chr ��Ҫ��ʾ���ַ���
					 size ��Ҫ��ʾ���ַ���С��
					 @arg 16,ѡ��8*16�����С�ַ���������ѡ��6*8�����ַ�
  * @retval None
  */
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, unsigned char size)
{
    unsigned char  c, i;

  	c = chr - ' ';
    if(x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if(size == 16)
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
					 size ��Ҫ��ʾ���ַ���С��

  * @retval None
  */
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char *chr, unsigned char size)
{
    unsigned char j = 0;
	
    while(chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], size);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y += 2;
        }
				j++;
    }
}

