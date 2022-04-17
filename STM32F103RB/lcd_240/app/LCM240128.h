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
8λ�˿ڶ�д����
����
1.���³�ʼ���˿�Ϊ����ģʽ
2.��ȡ�˿���ֵ 16 λ��ֻ��һ�ζ��������˿����ݣ�
3.�Զ������ݽ��д���õ�8λ��ֵ

д��
1.���³�ʼ���˿�Ϊ���ģʽ
2.��Ҫ�����8λ���ݽ��д���Ϊ16λ����
3.����˿���ֵ 16 λ������һ����������˿����ݣ�

*/
// PB8 - PB15
#define  Dirout (GPIOB->CRH = 0X33333333 )										//�������ģʽ
#define  Dirin  (GPIOB->CRH = 0x88888888 )//       ;GPIOB->ODR &= 0x00ff //��������,���Ѹ�λ
#define  Dataout(a) ( GPIOB->ODR = ( (GPIOB->ODR&0X00FF)|((a|0x0000)<<8) ) )	//�˿����8λ����a a��һ���޷���8λ����

																																							//�Ȱ�PB8-PB15���㣬����a����8λ��ֵ������aΪuchar�������Ȼ�һ��0x0000,���16λ���ٽ�������
#define  Datain() (uint8_t)(GPIOB->IDR>>8)



#define LCD_CUR_POS   0x21              //���λ������      
#define LCD_CGR_POS   0x22              //CGRAMƫ�õ�ַ����
#define LCD_ADR_POS   0x24              //��ַָ��λ��
#define LCD_TXT_STP   0x40              //�ı����׵�ַ
#define LCD_TXT_WID   0x41              //�ı������  
#define LCD_GRH_STP   0x42              //ͼ�����׵�ַ  
#define LCD_GRH_WID   0x43              //ͼ�������  
#define LCD_MOD_OR    0x80              //�ϳ���ʾ��ʽΪ"��"         
#define LCD_MOD_XOR   0x81              //�ϳ���ʾ��ʽΪ"���"
#define LCD_MOD_AND   0x82              //�ϳ���ʾ��ʽΪ"��"
#define LCD_MOD_TCH   0x83              //�ı�����
#define LCD_DIS_SW    0x90              //�ر���ʾ
#define LCD_DIS_MD1   0x90              //ͼ�� �ı� ��겻��ʾ ��겻��˸
#define LCD_DIS_MD2   0x9e              //ͼ�� �ı� �����ʾ ��겻��˸
#define LCD_DIS_MD3   0x9f              //ͼ�� �ı� �����ʾ �����˸
#define LCD_DIS_MD4   0x9c              //ͼ�� �ı� ��겻��ʾ �����˸
#define LCD_DIS_MD5   0x94              //ֻ��ʾ�ı�
#define LCD_DIS_MD6   0x98              //ֻ��ʾͼ��
#define LCD_CUR_SHP   0xA0              //�����״
#define LCD_CUR_SHP7  0xA7             //�����״7
#define LCD_AUT_WR    0xB0              //�Զ�д
#define LCD_AUT_RD    0xB1              //�Զ���
#define LCD_AUT_OVR   0xB2              //�Զ���/д�ر�
#define LCD_INC_WR    0xC0              //���ݶ� ��ַ��1
#define LCD_INC_RD    0xC1              //����д ��ַ��1
#define LCD_DEC_WR    0xC2              //����д ��ַ��1
#define LCD_DEC_RD    0xC3              //���ݶ� ��ַ��1
#define LCD_NOC_WR    0xC4              //����д ��ַ����
#define LCD_NOC_RD    0xC5              //���ݶ� ��ַ����
#define LCD_SCN_RD    0xE0              //����
#define LCD_SCN_CP    0xE8              //������
#define LCD_BIT_OP    0xF0              //λ����


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

//�ı�д�뺺��
void Writexthz(uint8_t x,uint8_t y,uint8_t a[][32],uint8_t num);
//�Զ����ַ�

void cgram(unsigned char a[][32],uint8_t num);


#endif
