#ifndef __HG240128_H__
#define __HG240128_H__


#define uchar uint8_t
#define uint  uint16_t

typedef unsigned           char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;



#define SET_WR  PCout(6) = 1           //WR:PC6
#define REST_WR PCout(6) = 0
#define SET_RD  PCout(7) = 1           //RD:PC7
#define REST_RD PCout(7) = 0  
#define SET_CE  PCout(8) = 1           //CE:PC8
#define REST_CE PCout(8) = 0 
#define SET_CD  PCout(9) = 1           //CD:PC9
#define REST_CD PCout(9) = 0


/*
8位端口读写操作
读：
1.重新初始化端口为输入模式
2.读取端口数值 16 位（只能一次读出整个端口数据）
3.对读出数据进行处理得到8位数值

写：
1.重新初始化端口为输出模式
2.对要输出的8位数据进行处理为16位数据
3.输出端口数值 16 位（智能一次输出整个端口数据）

*/
// PB8 - PB15
#define  Dirout (GPIOB->CRH = 0X33333333 )										//推挽输出模式
#define  Dirin  (GPIOB->CRH = 0x88888888 )//       ;GPIOB->ODR &= 0x00ff //上拉输入,并把高位
#define  Dataout(a) ( GPIOB->ODR = ( (GPIOB->ODR&0X00FF)|((a|0x0000)<<8) ) )	//端口输出8位数据a a是一个无符号8位参数

																																							//先把PB8-PB15清零，或上a左移8位的值，由于a为uchar，所以先或一个0x0000,变成16位，再进行左移
#define  Datain() (uint8_t)(GPIOB->IDR>>8)



#define LCD_CUR_POS   0x21              //光标位置设置      
#define LCD_CGR_POS   0x22              //CGRAM偏置地址设置
#define LCD_ADR_POS   0x24              //地址指针位置
#define LCD_TXT_STP   0x40              //文本区首地址
#define LCD_TXT_WID   0x41              //文本区宽度  
#define LCD_GRH_STP   0x42              //图形区首地址  
#define LCD_GRH_WID   0x43              //图形区宽度  
#define LCD_MOD_OR    0x80              //合成显示方式为"或"         
#define LCD_MOD_XOR   0x81              //合成显示方式为"异或"
#define LCD_MOD_AND   0x82              //合成显示方式为"与"
#define LCD_MOD_TCH   0x83              //文本特征
#define LCD_DIS_SW    0x90              //关闭显示
#define LCD_DIS_MD1   0x90              //图形 文本 光标不显示 光标不闪烁
#define LCD_DIS_MD2   0x9e              //图形 文本 光标显示 光标不闪烁
#define LCD_DIS_MD3   0x9f              //图形 文本 光标显示 光标闪烁
#define LCD_DIS_MD4   0x9c              //图形 文本 光标不显示 光标闪烁
#define LCD_DIS_MD5   0x94              //只显示文本
#define LCD_DIS_MD6   0x98              //只显示图形
#define LCD_CUR_SHP   0xA0              //光标形状
#define LCD_CUR_SHP7  0xA7             //光标形状7
#define LCD_AUT_WR    0xB0              //自动写
#define LCD_AUT_RD    0xB1              //自动读
#define LCD_AUT_OVR   0xB2              //自动读/写关闭
#define LCD_INC_WR    0xC0              //数据读 地址加1
#define LCD_INC_RD    0xC1              //数据写 地址加1
#define LCD_DEC_WR    0xC2              //数据写 地址减1
#define LCD_DEC_RD    0xC3              //数据读 地址减1
#define LCD_NOC_WR    0xC4              //数据写 地址不变
#define LCD_NOC_RD    0xC5              //数据读 地址不变
#define LCD_SCN_RD    0xE0              //屏读
#define LCD_SCN_CP    0xE8              //屏拷贝
#define LCD_BIT_OP    0xF0              //位操作


uint8_t LCD_TestStaBit01(void);
uint8_t LCD_TestStaBit02(void);
uint8_t LCD_TestStaBit03(void);
uint8_t LCD_TestStaBit06(void);
void LCD_Clear(void);
void LCD_Writedata(uint8_t);
void LCD_Writecom(uint8_t); 
void LCD_Writesingle(uint8_t,uint8_t);
void LCD_Writedouble(uint8_t,uint8_t,uint8_t);
void LCD_Text_Init(void);
void LCD_Writestring(uint8_t,uint8_t,uint8_t *);
void LCD_Writegraph(uint8_t ,uint8_t ,uint8_t ,uint8_t ,const uint8_t *);
void LCD_Writebar_ch(uint8_t,uint8_t,const uint8_t *);
void LCD_Writebar(uint16_t *);
void LCD_Welcome(void);

//文本写入汉字
void Writexthz(uint8_t x,uint8_t y,uint8_t a[][32],uint8_t num);
//自定义字符

void cgram(unsigned char a[][32],uint8_t num);


#endif
