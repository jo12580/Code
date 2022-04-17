#ifndef __KEY_H
#define __KEY_H

#include "stm32l0xx_hal.h"

//�����˰�������
typedef enum
{
    USER = 0,
    NOKEY
} KEY_VAL;

//�����˰����Ľṹ��
typedef struct
{
    uint8_t value;
    uint8_t status;
    void (*KEY_Scan)(void);
} KEY_STRUCT;

//������2*3������̶�д������
#define COLUMN1st_WRITE_HIGH   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define COLUMN1st_WRITE_LOW    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define COLUMN2nd_WRITE_HIGH   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define COLUMN2nd_WRITE_LOW 	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define COLUMN3rd_WRITE_HIGH 	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)
#define COLUMN3rd_WRITE_LOW 	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)
#define ROW1st_READ 					 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)
#define ROW2nd_READ 					 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)

//������2*3������̰���ֵ
#define B1 1
#define B2 2
#define B3 3
#define B4 4
#define B5 5
#define B6 6

/**
  * @brief ��������USER���غ�����
  * @param None
  * @retval None
  */
KEY_STRUCT *UserKey_Fun(void);


void KEY_Init(void);

/**
  * @brief �밴���йص����ų�ʼ��,�Ѿ����������б���ʼ����ɣ�������PB6��PB7��������Ϊ���룬PB1��PB0��PA8��������Ϊ�������ʼ���������͵�ƽ��
  * @param None
  * @retval None
  */
void Keyboard_Init(void);

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
uint8_t Keyboard_Scan(void);


#endif
