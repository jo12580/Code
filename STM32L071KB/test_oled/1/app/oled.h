/*************************************************
Copyright (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: oled.h
Author:      Version:   Date:2019.10.10
Description: OLED��ʾ���Ķ�д������ͷ�ļ��������˶�OLED����д����������
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#ifndef __OLED_H
#define __OLED_H


#define Max_Column	128
#define Max_Row		32


typedef enum
{
    TYPE_COMMAND = 0,
    TYPE_DATA = 0x40
} OLED_TYPE;

/**
  * @brief OLED��Ļ������
  * @param None
  * @retval None
  */
void OLED_Clear(void);

/**
  * @brief OLED��Ļ��ʼ����
  * @param None
  * @retval None
  */
void OLED_Init(void);

/**
  * @brief OLED��Ļ��ʾһ���ַ���
  * @param x ����ѡ��λ�ã�
					 y ����ѡ��λ�ã�
					 chr ��Ҫ��ʾ���ַ���
					 size ��Ҫ��ʾ���ַ���С��

  * @retval None
  */
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, unsigned char size);

/**
  * @brief OLED��Ļ��ʾһ���ַ�����
  * @param x ����ѡ��λ�ã�
					 y ����ѡ��λ�ã�
					 chr ��Ҫ��ʾ���ַ�����
					 size ��Ҫ��ʾ���ַ���С��

  * @retval None
  */
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char *chr, unsigned char size);


#endif
