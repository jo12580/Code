/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

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
 *
 */

#define GPIO_OUTPUT_IO_0    5
#define GPIO_OUTPUT_IO_1    6
#define GPIO_OUTPUT_IO_2    7
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1) |(1ULL<<GPIO_OUTPUT_IO_2))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     2
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

void init()
{
    gpio_config_t io_conf = {};//zero-initialize the config structure.
    io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;//set as output mode
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;//bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pull_down_en = 0; //disable pull-down mode
    io_conf.pull_up_en = 0;//disable pull-up mode
    gpio_config(&io_conf);//configure GPIO with the given settings
    io_conf.intr_type = GPIO_INTR_POSEDGE;//interrupt of rising edge
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;//bit mask of the pins, use GPIO4/5 here
    io_conf.mode = GPIO_MODE_INPUT;//set as input mode
    io_conf.pull_up_en = 1; //enable pull-up mode

    gpio_config(&io_conf);
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE); //change gpio intrrupt type for one pin

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));//create a queue to handle gpio event from isr
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);//start gpio task

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);//install gpio isr service
    
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);//hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
    
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);//remove isr handler for gpio number.
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);//hook isr handler for specific gpio pin again
    
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
}

void app_main(void)
{
    init();
    int cnt = 0;
    while(1)    //五颜六色的随机出现
    {
        printf("cnt: %d\n", cnt);
        vTaskDelay(1000 / portTICK_RATE_MS);
        cnt=rand()%2+0;
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt);  //设置引脚的电平，cnt取0、1
        cnt=rand()%2+0;
        gpio_set_level(GPIO_OUTPUT_IO_1, cnt);
        cnt=rand()%2+0;
        gpio_set_level(GPIO_OUTPUT_IO_2, cnt);
    }
}
