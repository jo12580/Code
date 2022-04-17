#ifndef __KEY_H
#define __KEY_H

#include "stm32l0xx_hal.h"

//定义了按键的类
typedef enum
{
    USER = 0,
    NOKEY
} KEY_VAL;

//定义了按键的结构体
typedef struct
{
    uint8_t value;
    uint8_t status;
    void (*KEY_Scan)(void);
} KEY_STRUCT;

//定义了2*3矩阵键盘读写操作宏
#define COLUMN1st_WRITE_HIGH   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define COLUMN1st_WRITE_LOW    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define COLUMN2nd_WRITE_HIGH   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define COLUMN2nd_WRITE_LOW 	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define COLUMN3rd_WRITE_HIGH 	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)
#define COLUMN3rd_WRITE_LOW 	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)
#define ROW1st_READ 					 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)
#define ROW2nd_READ 					 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)

//定义了2*3矩阵键盘按键值
#define B1 1
#define B2 2
#define B3 3
#define B4 4
#define B5 5
#define B6 6

/**
  * @brief 独立按键USER返回函数。
  * @param None
  * @retval None
  */
KEY_STRUCT *UserKey_Fun(void);


void KEY_Init(void);

/**
  * @brief 与按键有关的引脚初始化,已经在主函数中被初始化完成，其中行PB6、PB7均被配置为输入，PB1、PB0、PA8均被配置为输出。初始输出均输出低电平。
  * @param None
  * @retval None
  */
void Keyboard_Init(void);

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
uint8_t Keyboard_Scan(void);


#endif
