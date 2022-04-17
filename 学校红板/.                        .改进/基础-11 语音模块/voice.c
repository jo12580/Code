#include<voice.h>

void  delay_us(unsigned int i)
{
	while(i--);
}

void voice(unsigned char i)
{	
	unsigned char a;
	dat=0;  
	rest=0;
	delay_us(50);
	rest=1;					
	delay_us(50);
	rest=0;
    delay_us(50);
	dat=0;
	delay_us(50);
	
	for(a=0;a<i;a++)
       {
		dat=1;
		delay_us(50);
		dat=0;
		delay_us(50);   
	   }
	while (!busy);    
}