#include<reg52.h>
#include"key.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

unsigned char key_scanf(void);
void key(unsigned char a );
void delay(unsigned int i);

void main()
{
	unsigned char temp;
 while(1)
    {
        temp = key_scanf();
        if(temp != 0xff)
        {
            delay(5000);  //��������
            temp = key_scanf();
            if(temp)
            {
                //���밴��������
                key(temp);
                while(P3 != 0xff);  //�ȴ������ͷ�                        
            }
        }
    }

}

void delay(unsigned int i)
{
while(i--);
}