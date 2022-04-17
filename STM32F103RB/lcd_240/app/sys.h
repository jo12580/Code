#ifndef _SYS_H_
#define _SYS_H_
#include "stm32f1xx_it.h"
//#include "main.h"


/* 利用位带操作，实现单位操作 */
#define BITBAND( addr,bitnum ) ( (addr & 0XF0000000) + 0X2000000 + ( (addr & 0XFFFFF)<<5 )+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR( addr,bitnum ) MEM_ADDR(BITBAND(addr , bitnum))

// IO 映射
#define GPIOA_ODR_Addr	( GPIOA_BASE + 12 ) 
#define GPIOB_ODR_Addr	( GPIOB_BASE + 12 ) 
#define GPIOC_ODR_Addr	( GPIOC_BASE + 12 ) 
#define GPIOD_ODR_Addr	( GPIOD_BASE + 12 ) 
#define GPIOE_ODR_Addr	( GPIOE_BASE + 12 ) 
#define GPIOF_ODR_Addr	( GPIOF_BASE + 12 ) 
#define GPIOG_ODR_Addr	( GPIOG_BASE + 12 ) 

#define GPIOA_IDR_Addr	( GPIOA_BASE + 8 ) 
#define GPIOE_IDR_Addr	( GPIOE_BASE + 8 ) 

#define PAout(n)	BIT_ADDR( GPIOA_ODR_Addr,n )
#define PBout(n)	BIT_ADDR( GPIOB_ODR_Addr,n )
#define PCout(n)	BIT_ADDR( GPIOC_ODR_Addr,n )
#define PDout(n)	BIT_ADDR( GPIOD_ODR_Addr,n )
#define PEout(n)	BIT_ADDR( GPIOE_ODR_Addr,n )
#define PFout(n)	BIT_ADDR( GPIOF_ODR_Addr,n )
#define PGout(n)	BIT_ADDR( GPIOG_ODR_Addr,n )

#define PAin(n)	BIT_ADDR( GPIOA_IDR_Addr,n )
#define PEin(n)	BIT_ADDR( GPIOE_IDR_Addr,n )

#endif

