#ifndef __SPI_H
#define __SPI_H

#include "stm32l0xx.h"
#include "stdlib.h"

void spi_init(void);
void spi_send_byte(uint8_t byte);
uint8_t spi_recv_byte(void);


#endif


