#ifndef __KEY_H
#define __KEY_H

#include "gpio.h"
//unsigned int key(void);
uint8_t Row_Scan(void);
unsigned int key2(void);
void Keyboard_Init(void);
void key1(void);

extern unsigned int key_exti;
//extern unsigned int key_in;
#endif
