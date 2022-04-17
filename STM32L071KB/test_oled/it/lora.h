#ifndef __LORA_H__
#define __LORA_H__
void LORA_Init(void);
void LORA_Tx(unsigned char *pucBuf, unsigned char ucSize);
unsigned char LORA_Rx(unsigned char *pucBuf);
#endif /* __LORA_H__ */
