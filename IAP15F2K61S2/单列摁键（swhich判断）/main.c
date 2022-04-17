#include<reg52.h>
#include"key.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
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
            delay(5000);  //按键消抖
            temp = key_scanf();
            if(temp)
            {
                //进入按键处理函数
                key(temp);
                while(P3 != 0xff);  //等待按键释放                        
            }
        }
    }

}

void delay(unsigned int i)
{
while(i--);
}