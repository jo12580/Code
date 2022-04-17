#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h" 

void key_init(void);
//unsigned int key_scan(void);
unsigned int key0_scan(void);
unsigned int key1_scan(void);
unsigned int key2_scan(void);

#endif 
