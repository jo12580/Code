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

#include "stm32l0xx_hal.h"

#define Max_Column	128
#define Max_Row		32


typedef enum
{
    TYPE_COMMAND = 0,
    TYPE_DATA
} OLED_TYPE;

/**
  * @brief OLED��Ļ��Դ���ؿ��ơ�
  * @param status
					 @arg ENABLE :��OLED��Դ��
					 @arg DSIABLE:�ر�OLED��Դ��

  * @retval None
  */
void OLED_PowerControl(uint8_t Status);

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
					 Char_Size ��Ҫ��ʾ���ַ���С��

  * @retval None
  */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);

/**
  * @brief OLED��Ļ��ʾһ���ַ�����
  * @param x ����ѡ��λ�ã�
					 y ����ѡ��λ�ã�
					 chr ��Ҫ��ʾ���ַ�����
					 Char_Size ��Ҫ��ʾ���ַ���С��

  * @retval None
  */
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size);


#endif
