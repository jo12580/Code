#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include "rgb_led.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

void IRAM_ATTR gpio_isr_handler(void* arg);
void init_led(void);    //初始化led程序，需放在main函数里
void gpio_task_example(void* arg);  //led处理函数，用来打印输出引脚

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 */
#define GPIO_OUTPUT_IO_0    5
#define GPIO_OUTPUT_IO_1    6
#define GPIO_OUTPUT_IO_2    7
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1) |(1ULL<<GPIO_OUTPUT_IO_2))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     2
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

#endif 
