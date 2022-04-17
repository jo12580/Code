#ifndef __OLED_H
#define __OLED_H

#include "stm32l0xx.h"
#include "stdlib.h"

#define Max_Column	128
#define Max_Row		32

/****************OLED IIC�˿ڶ���****************/
/****************OLED Control pins***************/
/****************����ʵ��ʹ������޸�************/
#define SCLK_Pin       GPIO_PIN_8
#define DB_SCLK_Port   GPIOA
#define SDA_Pin        GPIO_PIN_4
#define DB_SDA_Port    GPIOB
/************************************************/


#define OLED_SCLK_Low     GPIOA->BRR = ((uint16_t)0x0100U)  //SDA IIC�ӿڵ�ʱ���ź�
#define OLED_SCLK_High    GPIOA->BSRR = ((uint16_t)0x0100U)

#define OLED_SDA_Low      GPIOB->BRR = ((uint16_t)0x0010U) //SCL IIC�ӿڵ������ź�
#define OLED_SDA_High     GPIOB->BSRR = ((uint16_t)0x0010U)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�ⲿ����
/************************�ⲿ����**************************/
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_On(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void OLED_ShowChar1(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *p, uint8_t Char_Size);
void OLED_ShowString1(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
/**********************************************************/

/***********************�ڲ�����***************************/
//void IIC_Start(void);
//void IIC_Stop(void);
//void Write_IIC_Command(unsigned char IIC_Command);
//void Write_IIC_Data(unsigned char IIC_Data);
//void Write_IIC_Byte(unsigned char IIC_Byte);
//void IIC_Wait_Ack(void);
//void OLED_DrawPoint(u8 x,u8 y,u8 t);
//void OLED_WR_Byte(unsigned dat,unsigned cmd);
//void OLED_Set_Pos(unsigned char x, unsigned char y);
//void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
/**********************************************************/
#endif

