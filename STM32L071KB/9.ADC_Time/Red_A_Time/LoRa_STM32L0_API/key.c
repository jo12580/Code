/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: key.c
Author:      Version:   Date:2019.10.10
Description: USER������ɨ�躯����2*3�������ɨ�躯����
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#include "key.h"

KEY_STRUCT UserKey;

/**
  * @brief ��������USER���غ�����
  * @param None
  * @retval None
  */
KEY_STRUCT *UserKey_Fun()
{
    return &UserKey;
}

/**
  * @brief ��������USERɨ�衣
  * @param None
  * @retval None
  */
void UserKey_Scan(void)
{
    uint8_t key_temp = 0;

    key_temp = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14);

    switch(UserKey.status)
    {
    case 0:
        if(key_temp == GPIO_PIN_RESET)
        {
            UserKey.status = 1;
        }
        else
        {
            ;
        }
        break;
    case 1:
        if(key_temp == GPIO_PIN_RESET)
        {
            UserKey.status = 2;
            UserKey.value = USER;
        }
        else
        {
            ;
        }
        break;
    case 2:
        if(key_temp == GPIO_PIN_SET)
        {
            UserKey.status = 0;
        }
        break;
    default:
        break;
    }
}

/**
  * @brief ��������USER��ʼ����
  * @param None
  * @retval None
  */
void USERKey_Init(void)
{
    UserKey.KEY_Scan = UserKey_Scan;
    UserKey.value = NOKEY;
    UserKey.status = 0;
}

/**
  * @brief �밴���йص����ų�ʼ��,�Ѿ����������б���ʼ����ɣ�
					 ������PB6��PB7��������Ϊ���룬PB1��PB0��PA8��������
					 Ϊ�������ʼ���������͵�ƽ��
  * @param None
  * @retval None
  */
void Keyboard_Init(void)
{

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

}

/**
  * @brief ��ֵɨ��
  * @param None
  * @retval RowValue_Return �з���ֵ
						@arg 0 û���б���⵽
						@arg 1 ��1����⵽
						@arg 2 ��2����⵽
  */
uint8_t count = 0;
uint8_t Row_Scan(void)
{
    uint8_t Row1st_Value = 0;
    uint8_t Row2nd_Value = 0;

    uint8_t RowValue_Return = 0;

    Row2nd_Value = ROW2nd_READ;
    Row1st_Value = ROW1st_READ;


    if (Row1st_Value == 1)
    {
        count++;
        if (count == 3)        //5�μ�����⵽���룬�϶�Ϊ������
        {
            count = 0;
            RowValue_Return = 1;
        }
    }

    if (Row2nd_Value == 1)
    {
        count++;
        if (count == 3)        //5�μ�����⵽���룬�϶�Ϊ������
        {
            count = 0;
            RowValue_Return = 2;
        }
    }

    return (RowValue_Return);
}


/**
  * @brief �������ɨ���ֵ
  * @param None
  * @retval Key_Value ���صļ�ֵ
						@arg B1 ����1
						@arg B2 ����2
						@arg B3 ����3
						@arg B4 ����4
						@arg B5 ����5
						@arg B6 ����6
  */
unsigned int key2(void)
{
    unsigned int Key_Value = 0;
    uint8_t key = 0;

    COLUMN3rd_WRITE_LOW;
    COLUMN1st_WRITE_HIGH;
    key = Row_Scan();
    if (key != 0)
    {
        switch (key)
        {
        case 1:
            Key_Value = 11;
            break;
        case 2:
            Key_Value = 21;
            break;
        default:
            break;
        }
    }
    COLUMN1st_WRITE_LOW;

    COLUMN2nd_WRITE_HIGH;
    key = Row_Scan();
    if (key != 0)
    {
        switch (key)
        {
        case 1:
            Key_Value = 12;
            break;
        case 2:
            Key_Value = 22;
            break;
        default:
            break;
        }
    }
    COLUMN2nd_WRITE_LOW;

    COLUMN3rd_WRITE_HIGH;
    key = Row_Scan();
    if (key != 0)
    {
        switch (key)
        {
        case 1:
            Key_Value = 13;
            break;
        case 2:
            Key_Value = 23;
            break;
        default:
            break;
        }
    }
    COLUMN3rd_WRITE_LOW;

    return (Key_Value);
}

