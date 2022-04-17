#ifndef __DELAY_H__
#define __DELAY_H__

void delay15ms(void);
void delay5ms(void);   

#endif

void delay15ms(void)   //?? 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=28;b>0;b--)
            for(a=52;a>0;a--);
}

void delay5ms(void)   //?? 0us
{
    unsigned char a,b;
    for(b=185;b>0;b--)
        for(a=12;a>0;a--);
}
