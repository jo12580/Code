#include<reg52.h>
																	/*
																		摁键的功能
																	*/
void key(unsigned char temp )
{
					P2 = 0x80;
					switch(temp)
					{
						case 1:   //S7
							P0 =0x00;						//输入想实现的功能
							break;
						
						case 2:  //S6
							P0=0xff;
						break;
						
						case 3:	//S5
							P0=0x80;
						break;
						
//						case 4:	//S4
//			
					}
}

unsigned char key_scanf(void)
{
		unsigned char key_value= 0xff;
		unsigned char 	temp;
		temp=P3&0x0f;
	switch(temp)
	{
				case 0x0e:
            key_value = 1;  //S7
            break;    
        case 0x0d:
            key_value = 2;  //S6
            break;    
        case 0x0b:
            key_value = 3;  //S5
            break;    
        case 0x07:
            key_value = 4;  //S4
            break;  
	}
	return key_value ;
}