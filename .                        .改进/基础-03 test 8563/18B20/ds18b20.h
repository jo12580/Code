#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <reg52.h>
#include "predefine.h"

sbit   DQ = P2^0;					   //DS18B20数据端

#define read_rom   	  0x33
#define match_rom     0x55
#define skip_rom      0xcc
#define search_rom    0xf0
#define alarm_search  0xec
#define write_scrctchpad  0x4e
#define read_scrctchpad   0xbe
#define copy_scrctchpad   0x48
#define convert_t         0x44
#define recall_e2         0xb8
#define read_power_supply 0xb4

//函数声明
void	ds18b20_set_resolution(uchar res);	 //分辨率设置函数
void    ds18b20_read_temperature(void); 		   //读取温度函数
void    ds18b20_display_temperature(void);

#endif