#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"


//��ʱ����
void Delay_Oled(void)
{
    uint32_t i = 0x7fff;
    while(i--);
}


//������ʼ����
void IIC_Start(void)
{
    OLED_SCLK_High;
    OLED_SDA_High;
    OLED_SDA_Low;
    OLED_SCLK_Low;
}


//���߽�������
void IIC_Stop(void)
{
    OLED_SDA_Low;
    OLED_SCLK_High;
    OLED_SDA_High;
}


//�ȴ�Ӧ��
void IIC_Wait_Ack(void)
{
    OLED_SDA_High;
    OLED_SCLK_High;
    OLED_SCLK_Low;
}

//дһ���ֽ�
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


//д����
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


//д����
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

//dat��д�������������
//cmd����Ϊ0ʱ��д���Ϊ1ʱ��д����
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


//��������
//xΪҪ��ʾ����ʼ�е�ַ����ΧΪ0~127
//yΪҪ��ʾ��ҳ��ַ,��ΧΪ0~3
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD); //������ʼҳ��ַ
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD); //��ʼ�е�ַ�ĸ�4λ
    OLED_WR_Byte((x & 0x0f), OLED_CMD); //��ʼ�е�ַ�ĵ���λ
}

//����OLED��ʾ
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

//��������,������,������Ļ
//�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 4; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //����ҳ��ַ��0~3��
        OLED_WR_Byte (0x00, OLED_CMD);     //������ʾλ�á��е͵�ַ
        OLED_WR_Byte (0x10, OLED_CMD);     //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)OLED_WR_Byte(0, OLED_DATA);
    } //������ʾ
}

//��ʾȫ�����ص�
void OLED_On(void)
{
    uint8_t i, n;
    for(i = 0; i < 4; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //����ҳ��ַ��0~3��
        OLED_WR_Byte (0x00, OLED_CMD);     //������ʾλ�á��е͵�ַ
        OLED_WR_Byte (0x10, OLED_CMD);     //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)OLED_WR_Byte(0xff, OLED_DATA);
    } //������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~3
//size:ѡ�����壬��ѡΪ8��16������8Ϊ6*8������ʾ��16Ϊ8*16������ʾ��
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //�õ�ƫ�ƺ��ֵ
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


//��ָ��λ����ʾһ�������ַ�,���������ַ�
//x:0~127
//y:0~3
//size:ѡ������ 16/12
void OLED_ShowChar1(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //�õ�ƫ�ƺ��ֵ
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

//m^n����
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)result *= m;
    return result;
}
//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
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
//��ʾһ�������ַ���
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

//��ʾһ���ַ��Ŵ�
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

//��ʾ����
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
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��3*****************/
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


//��ʼ��SSD1306����������ʼ��Һ����
void OLED_Init(void)
{
    GPIO_InitTypeDef  GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE(); //����GPIOAʱ��
    GPIO_Initure.Pin = SCLK_Pin; //PA8
    GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(DB_SCLK_Port, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

    __HAL_RCC_GPIOB_CLK_ENABLE(); //����GPIOAʱ��
    GPIO_Initure.Pin = SDA_Pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    HAL_GPIO_Init(DB_SDA_Port, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    Delay_Oled(); //�ϵ���ʱ����

    OLED_WR_Byte(0xAE, OLED_CMD); //�ر���ʾ

    OLED_WR_Byte(0xD5, OLED_CMD); //����ʱ�ӷ�Ƶ���ӣ���Ƶ��
    OLED_WR_Byte(0x80, OLED_CMD); //[3:0],��Ƶ����;[7:4],��Ƶ��

    OLED_WR_Byte(0xA8, OLED_CMD); //��������·��
    OLED_WR_Byte(0x1F, OLED_CMD); //Ĭ�� 0X1F(1/64)

    OLED_WR_Byte(0xD3, OLED_CMD); //������ʾƫ��
    OLED_WR_Byte(0x00, OLED_CMD); //Ĭ��Ϊ 0


    OLED_WR_Byte(0x40, OLED_CMD); //������ʾ��ʼ�� [5:0],����.
    OLED_WR_Byte(0xB0, OLED_CMD);

    OLED_WR_Byte(0x8D, OLED_CMD); //��ɱ�����
    OLED_WR_Byte(0x14, OLED_CMD); //bit2������

    OLED_WR_Byte(0xA1, OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC8, OLED_CMD);
    OLED_WR_Byte(0xDA, OLED_CMD); //���� COM Ӳ����������
    OLED_WR_Byte(0x00, OLED_CMD); //[5:4]����
    OLED_WR_Byte(0x81, OLED_CMD); //�Աȶ�����
    OLED_WR_Byte(0xFF, OLED_CMD); //1~255;(��������,Խ��Խ��)
    OLED_WR_Byte(0xD9, OLED_CMD); //����Ԥ�������
    OLED_WR_Byte(0x1F, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB, OLED_CMD); //���� VCOMH ��ѹ����
    OLED_WR_Byte(0x40, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WR_Byte(0xA4, OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)

    OLED_Clear();//����
    OLED_WR_Byte(0xAF, OLED_CMD); //������ʾ
}




