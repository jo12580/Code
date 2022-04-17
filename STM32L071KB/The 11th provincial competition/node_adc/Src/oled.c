/*************************************************
Copyright (c)Guoxinchangtian Technologies Co., Ltd. 2008-2019. All rights reserved.
File name: oled.c
Author:      Version:   Date:2019.10.10
Description: 实现对LoRa模块的OLED显示屏的读写操作。
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
  * @brief OLED屏幕电源开关控制。
  * @param status
					 @arg ENABLE :打开OLED电源；
					 @arg DSIABLE:关闭OLED电源；

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
  * @brief OLED屏幕写数据。
  * @param type OLED的屏幕命令类型；
					 @arg TYPE_COMMAND :打开OLED电源；
					 @arg TYPE_DATA:关闭OLED电源；

					 data 发送给OLED屏幕的数据；

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
  * @brief OLED屏幕显示开启。
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
  * @brief OLED屏幕显示关闭。
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
  * @brief OLED屏幕写数据。
  * @param x 0-127，选择x方向位置；
					 y 0-3，选择y方向位置；
  * @retval None
  */
void OLED_SetPos(uint8_t x, uint8_t y)
{
    OLED_Write(TYPE_COMMAND, 0xb0 + y);
    OLED_Write(TYPE_COMMAND, ((x & 0xf0) >> 4) | 0x10);
    OLED_Write(TYPE_COMMAND, (x & 0x0f));
}

/**
  * @brief OLED屏幕清屏。
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
  * @brief OLED屏幕显示一个字符。
  * @param x 横向选择位置；
					 y 纵向选择位置；
					 chr 需要显示的字符；
					 Char_Size 需要显示的字符大小；
					 @arg 16,选择8*16点阵大小字符。其他均选择6*8点阵字符


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
  * @brief OLED屏幕显示一个字符串。
  * @param x 横向选择位置；
					 y 纵向选择位置；
					 chr 需要显示的字符串；
					 Char_Size 需要显示的字符大小；

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
  * @brief OLED屏幕初始化。
  * @param None
  * @retval None
  */
void OLED_Init(void)
{
    OLED_Write(TYPE_COMMAND, 0xAE);//关闭显示

    OLED_Write(TYPE_COMMAND, 0xD5);//设置时钟分频因子，震荡频率
    OLED_Write(TYPE_COMMAND, 0x80);//[3:0],分频因子;[7:4],震荡频率

    OLED_Write(TYPE_COMMAND, 0xA8);//设置驱动路数
    OLED_Write(TYPE_COMMAND, 0x1F);//默认 0X1F(1/64)

    OLED_Write(TYPE_COMMAND, 0xD3);//设置显示偏移
    OLED_Write(TYPE_COMMAND, 0x00);//默认为 0


    OLED_Write(TYPE_COMMAND, 0x40); //设置显示开始行 [5:0],行数.
    OLED_Write(TYPE_COMMAND, 0xB0);

    OLED_Write(TYPE_COMMAND, 0x8D);//电荷泵设置
    OLED_Write(TYPE_COMMAND, 0x14);//bit2，开启

    OLED_Write(TYPE_COMMAND, 0xA1);//段重定义设置,bit0:0,0->0;1,0->127;
    OLED_Write(TYPE_COMMAND, 0xC8);

    OLED_Write(TYPE_COMMAND, 0xDA);//设置 COM 硬件引脚配置
    OLED_Write(TYPE_COMMAND, 0x00);//[5:4]配置
    OLED_Write(TYPE_COMMAND, 0x81);//对比度设置
    OLED_Write(TYPE_COMMAND, 0xFF);//1~255;(亮度设置,越大越亮)
    OLED_Write(TYPE_COMMAND, 0xD9);//设置预充电周期
    OLED_Write(TYPE_COMMAND, 0x1F);//[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_Write(TYPE_COMMAND, 0xDB);//设置 VCOMH 电压倍率
    OLED_Write(TYPE_COMMAND, 0x40);//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_Write(TYPE_COMMAND, 0xA4);//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)

    OLED_Clear();//清屏
    OLED_Write(TYPE_COMMAND, 0xAF);//开启显示
}
