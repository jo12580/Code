#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"


//延时函数
void Delay_Oled(void)
{
    uint32_t i = 0x7fff;
    while(i--);
}


//总线起始条件
void IIC_Start(void)
{
    OLED_SCLK_High;
    OLED_SDA_High;
    OLED_SDA_Low;
    OLED_SCLK_Low;
}


//总线结束条件
void IIC_Stop(void)
{
    OLED_SDA_Low;
    OLED_SCLK_High;
    OLED_SDA_High;
}


//等待应答
void IIC_Wait_Ack(void)
{
    OLED_SDA_High;
    OLED_SCLK_High;
    OLED_SCLK_Low;
}

//写一个字节
void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m, da;
    da = IIC_Byte;
    OLED_SCLK_Low;
    for(i = 0; i < 8; i++)
    {
        m = da;
        m = m & 0x80;
        if(m == 0x80)
        {
            OLED_SDA_High;
        }
        else
        {
            OLED_SDA_Low;
        }
        da = da << 1;
        OLED_SCLK_High;
        OLED_SCLK_Low;
    }
}


//写命令
void Write_IIC_Command(unsigned char IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(0x78); //Slave address,SA0=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00); //write command
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}


//写数据
void Write_IIC_Data(unsigned char IIC_Data)
{
    IIC_Start();
    Write_IIC_Byte(0x78);//D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);//write data
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

//dat：写入命令或者数据
//cmd：当为0时，写命令，为1时，写数据
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
    if(cmd)
    {
        Write_IIC_Data(dat);
    }
    else
    {
        Write_IIC_Command(dat);
    }
}


//坐标设置
//x为要显示的起始列地址，范围为0~127
//y为要显示的页地址,范围为0~3
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD); //设置起始页地址
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD); //起始列地址的高4位
    OLED_WR_Byte((x & 0x0f), OLED_CMD); //起始列地址的低四位
}

//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

//关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕
//是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 4; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //设置页地址（0~3）
        OLED_WR_Byte (0x00, OLED_CMD);     //设置显示位置—列低地址
        OLED_WR_Byte (0x10, OLED_CMD);     //设置显示位置—列高地址
        for(n = 0; n < 128; n++)OLED_WR_Byte(0, OLED_DATA);
    } //更新显示
}

//显示全部像素点
void OLED_On(void)
{
    uint8_t i, n;
    for(i = 0; i < 4; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //设置页地址（0~3）
        OLED_WR_Byte (0x00, OLED_CMD);     //设置显示位置—列低地址
        OLED_WR_Byte (0x10, OLED_CMD);     //设置显示位置—列高地址
        for(n = 0; n < 128; n++)OLED_WR_Byte(0xff, OLED_DATA);
    } //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~3
//size:选择字体，可选为8和16（其中8为6*8点阵显示，16为8*16点阵显示）
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //得到偏移后的值
    if(x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if(Char_Size == 16)
    {
        OLED_Set_Pos(x, y);
        for(i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for(i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x, y);
        for(i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);
    }
}


//在指定位置显示一个反白字符,包括部分字符
//x:0~127
//y:0~3
//size:选择字体 16/12
void OLED_ShowChar1(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //得到偏移后的值
    if(x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if(Char_Size == 16)
    {
        OLED_Set_Pos(x, y);
        for(i = 0; i < 8; i++)
            OLED_WR_Byte(~F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for(i = 0; i < 8; i++)
            OLED_WR_Byte(~F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x, y);
        for(i = 0; i < 6; i++)
            OLED_WR_Byte(~F6x8[c][i], OLED_DATA);
    }
}

//m^n函数
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)result *= m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for(t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2)*t, y, ' ', size2);
                continue;
            }
            else enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2)*t, y, temp + '0', size2);
    }
}
//显示一个反白字符串
void OLED_ShowString1(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar1(x, y, chr[j], Char_Size);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

//显示一个字符号串
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size)
{
    unsigned char j = 0;
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
}

//显示汉字
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t, adder = 0;
    OLED_Set_Pos(x, y);
    for(t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for(t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～3*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if(y1 % 8 == 0)
    {
        y = y1 / 8;
    }
    else
    {
        y = y1 / 8 + 1;
    }
    for(y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for(x = x0; x < x1; x++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}


//初始化SSD1306控制器，初始化液晶屏
void OLED_Init(void)
{
    GPIO_InitTypeDef  GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE(); //开启GPIOA时钟
    GPIO_Initure.Pin = SCLK_Pin; //PA8
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(DB_SCLK_Port, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOA时钟
    GPIO_Initure.Pin = SDA_Pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    HAL_GPIO_Init(DB_SDA_Port, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    Delay_Oled(); //上电延时函数

    OLED_WR_Byte(0xAE, OLED_CMD); //关闭显示

    OLED_WR_Byte(0xD5, OLED_CMD); //设置时钟分频因子，震荡频率
    OLED_WR_Byte(0x80, OLED_CMD); //[3:0],分频因子;[7:4],震荡频率

    OLED_WR_Byte(0xA8, OLED_CMD); //设置驱动路数
    OLED_WR_Byte(0x1F, OLED_CMD); //默认 0X1F(1/64)

    OLED_WR_Byte(0xD3, OLED_CMD); //设置显示偏移
    OLED_WR_Byte(0x00, OLED_CMD); //默认为 0


    OLED_WR_Byte(0x40, OLED_CMD); //设置显示开始行 [5:0],行数.
    OLED_WR_Byte(0xB0, OLED_CMD);

    OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵设置
    OLED_WR_Byte(0x14, OLED_CMD); //bit2，开启

    OLED_WR_Byte(0xA1, OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC8, OLED_CMD);
    OLED_WR_Byte(0xDA, OLED_CMD); //设置 COM 硬件引脚配置
    OLED_WR_Byte(0x00, OLED_CMD); //[5:4]配置
    OLED_WR_Byte(0x81, OLED_CMD); //对比度设置
    OLED_WR_Byte(0xFF, OLED_CMD); //1~255;(亮度设置,越大越亮)
    OLED_WR_Byte(0xD9, OLED_CMD); //设置预充电周期
    OLED_WR_Byte(0x1F, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB, OLED_CMD); //设置 VCOMH 电压倍率
    OLED_WR_Byte(0x40, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WR_Byte(0xA4, OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)

    OLED_Clear();//清屏
    OLED_WR_Byte(0xAF, OLED_CMD); //开启显示
}




