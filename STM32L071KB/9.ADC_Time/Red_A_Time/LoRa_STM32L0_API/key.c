/*************************************************
Copyright  (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: key.c
Author:      Version:   Date:2019.10.10
Description: USER单按键扫描函数和2*3矩阵键盘扫描函数。
Others:
History:

1. Date:
Author:
Modification:
*************************************************/

#include "key.h"

KEY_STRUCT UserKey;

/**
  * @brief 独立按键USER返回函数。
  * @param None
  * @retval None
  */
KEY_STRUCT *UserKey_Fun()
{
    return &UserKey;
}

/**
  * @brief 独立按键USER扫描。
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
  * @brief 独立按键USER初始化。
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
  * @brief 与按键有关的引脚初始化,已经在主函数中被初始化完成，
					 其中行PB6、PB7均被配置为输入，PB1、PB0、PA8均被配置
					 为输出。初始输出均输出低电平。
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
  * @brief 行值扫描
  * @param None
  * @retval RowValue_Return 行返回值
						@arg 0 没有行被检测到
						@arg 1 行1被检测到
						@arg 2 行2被检测到
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
        if (count == 3)        //5次检测均检测到输入，认定为有输入
        {
            count = 0;
            RowValue_Return = 1;
        }
    }

    if (Row2nd_Value == 1)
    {
        count++;
        if (count == 3)        //5次检测均检测到输入，认定为有输入
        {
            count = 0;
            RowValue_Return = 2;
        }
    }

    return (RowValue_Return);
}


/**
  * @brief 矩阵键盘扫描键值
  * @param None
  * @retval Key_Value 返回的键值
						@arg B1 按键1
						@arg B2 按键2
						@arg B3 按键3
						@arg B4 按键4
						@arg B5 按键5
						@arg B6 按键6
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

